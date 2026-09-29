#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_task_wdt.h>

// ======================================================
// Hardware
// ======================================================

constexpr uint8_t PIN_LED = 25;
constexpr uint8_t PIN_BUTTON = 26;
constexpr uint8_t PIN_BUZZER = 27;

constexpr uint8_t BUZZER_ON = HIGH;
constexpr uint8_t BUZZER_OFF = LOW;

// ======================================================
// Configuração
// ======================================================

constexpr uint8_t ALARM_COUNT = 10;

constexpr unsigned long DEBOUNCE_MS = 40;
constexpr unsigned long CONFIG_LONG_PRESS_MS = 3000;
constexpr unsigned long CONFIG_TIMEOUT_MS = 10UL * 60UL * 1000UL;

const char* AP_SSID = "MedicineAlarm-Setup";
const char* AP_PASSWORD = "remedio123";

// ======================================================
// Alarmes
// ======================================================

struct AlarmSlot {
  uint8_t hour;
  uint8_t minute;
  uint8_t enabled;
};

AlarmSlot alarms[ALARM_COUNT];

// ======================================================
// Estado
// ======================================================

enum class DeviceState {
  IDLE,
  ALARM
};

DeviceState state = DeviceState::IDLE;

bool configActive = false;

Preferences preferences;
WebServer server(80);

uint32_t acknowledgementCount = 0;

unsigned long lastConfigActivity = 0;

// ======================================================
// Botão / debounce
// ======================================================

int lastRawButtonState = HIGH;
int stableButtonState = HIGH;

unsigned long lastButtonChange = 0;
unsigned long buttonPressStarted = 0;

bool longPressHandled = false;

// ======================================================
// Persistência
// ======================================================

void printAlarms() {
  Serial.println();
  Serial.println("------ ALARMES SALVOS ------");

  for (uint8_t i = 0; i < ALARM_COUNT; i++) {
    Serial.printf(
      "[%02u] %02u:%02u | %s\n",
      i + 1,
      alarms[i].hour,
      alarms[i].minute,
      alarms[i].enabled ? "ATIVO" : "desativado"
    );
  }

  Serial.println("----------------------------");
}

void saveAlarms() {
  size_t written = preferences.putBytes(
    "alarms",
    alarms,
    sizeof(alarms)
  );

  Serial.printf(
    "[NVS] Alarmes salvos: %u bytes\n",
    (unsigned int)written
  );
}

void createDefaultAlarms() {
  Serial.println("[NVS] Criando configuração inicial.");

  for (uint8_t i = 0; i < ALARM_COUNT; i++) {
    alarms[i].hour = 8 + i;
    alarms[i].minute = 0;
    alarms[i].enabled = 0;
  }

  saveAlarms();
}

void loadAlarms() {
  size_t storedSize = preferences.getBytesLength("alarms");

  if (storedSize != sizeof(alarms)) {
    createDefaultAlarms();
  } else {
    preferences.getBytes(
      "alarms",
      alarms,
      sizeof(alarms)
    );

    Serial.println("[NVS] Alarmes carregados.");
  }

  printAlarms();
}

// ======================================================
// Alarme físico
// ======================================================

void enterIdle() {
  state = DeviceState::IDLE;

  digitalWrite(PIN_BUZZER, BUZZER_OFF);

  Serial.println("[STATE] IDLE");
}

void enterAlarm() {
  if (state == DeviceState::ALARM) {
    return;
  }

  state = DeviceState::ALARM;

  digitalWrite(PIN_BUZZER, BUZZER_ON);

  Serial.println();
  Serial.println("==============================");
  Serial.println("       ALARME ATIVO");
  Serial.println("==============================");
  Serial.println("Pressione o botao para reconhecer.");
}

void acknowledgeAlarm() {
  digitalWrite(PIN_BUZZER, BUZZER_OFF);

  acknowledgementCount++;

  preferences.putUInt(
    "ack_count",
    acknowledgementCount
  );

  Serial.println("[BUTTON] Lembrete reconhecido.");

  Serial.printf(
    "[NVS] Total de reconhecimentos: %u\n",
    acknowledgementCount
  );

  enterIdle();
}

// ======================================================
// LED
// ======================================================

void updateLed() {
  unsigned long now = millis();

  if (state == DeviceState::ALARM) {
    digitalWrite(PIN_LED, HIGH);
    return;
  }

  if (configActive) {
    // Pisca rápido em modo configuração
    digitalWrite(
      PIN_LED,
      ((now / 500) % 2) ? HIGH : LOW
    );

    return;
  }

  // Pequeno "heartbeat" para indicar que o aparelho está vivo
  digitalWrite(
    PIN_LED,
    (now % 2000 < 80) ? HIGH : LOW
  );
}

// ======================================================
// Interface Web
// ======================================================

String buildWebPage() {
  String html;

  html.reserve(7000);

  html += R"HTML(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>MedicineAlarm</title>

<style>
body {
  font-family: sans-serif;
  max-width: 600px;
  margin: auto;
  padding: 20px;
}

h1 {
  margin-bottom: 5px;
}

.alarm {
  display: flex;
  gap: 10px;
  align-items: center;
  padding: 10px 0;
  border-bottom: 1px solid #ddd;
}

input[type=number] {
  width: 65px;
  font-size: 18px;
  padding: 6px;
}

button {
  width: 100%;
  font-size: 18px;
  padding: 14px;
  margin-top: 20px;
}
</style>

</head>

<body>

<h1>MedicineAlarm</h1>
<p>Configuração local de lembretes</p>

<form action="/save" method="POST">
)HTML";

  for (uint8_t i = 0; i < ALARM_COUNT; i++) {

    html += "<div class='alarm'>";

    html += "<strong>";
    html += String(i + 1);
    html += "</strong>";

    html += "<input type='number' min='0' max='23' name='h";
    html += String(i);
    html += "' value='";
    html += String(alarms[i].hour);
    html += "'>";

    html += ":";

    html += "<input type='number' min='0' max='59' name='m";
    html += String(i);
    html += "' value='";
    html += String(alarms[i].minute);
    html += "'>";

    html += "<label>";

    html += "<input type='checkbox' name='e";
    html += String(i);
    html += "' ";

    if (alarms[i].enabled) {
      html += "checked";
    }

    html += "> ativo";

    html += "</label>";

    html += "</div>";
  }

  html += R"HTML(

<button type="submit">
Salvar horários
</button>

</form>

<form action="/test" method="POST">

<button type="submit">
Testar alarme agora
</button>

</form>

<p>
Para sair do modo de configuração,
segure o botão físico por 3 segundos.
</p>

</body>
</html>
)HTML";

  return html;
}

void handleRoot() {
  lastConfigActivity = millis();

  server.send(
    200,
    "text/html; charset=utf-8",
    buildWebPage()
  );
}

void handleSave() {
  lastConfigActivity = millis();

  for (uint8_t i = 0; i < ALARM_COUNT; i++) {

    String hourName = "h" + String(i);
    String minuteName = "m" + String(i);
    String enabledName = "e" + String(i);

    int hour = server.arg(hourName).toInt();
    int minute = server.arg(minuteName).toInt();

    if (hour < 0 || hour > 23) {
      hour = 0;
    }

    if (minute < 0 || minute > 59) {
      minute = 0;
    }

    alarms[i].hour = hour;
    alarms[i].minute = minute;

    alarms[i].enabled =
      server.hasArg(enabledName) ? 1 : 0;
  }

  saveAlarms();
  printAlarms();

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleTestAlarm() {
  lastConfigActivity = millis();

  enterAlarm();

  server.sendHeader("Location", "/");
  server.send(303);
}

void setupWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/test", HTTP_POST, handleTestAlarm);

  server.onNotFound([]() {
    server.sendHeader("Location", "/");
    server.send(302);
  });
}

// ======================================================
// Wi-Fi AP
// ======================================================

void startConfigMode() {
  if (configActive) {
    return;
  }

  Serial.println();
  Serial.println("[CONFIG] Iniciando Wi-Fi...");

  WiFi.mode(WIFI_AP);

  bool ok = WiFi.softAP(
    AP_SSID,
    AP_PASSWORD
  );

  if (!ok) {
    Serial.println("[CONFIG] ERRO ao criar AP.");
    return;
  }

  server.begin();

  configActive = true;
  lastConfigActivity = millis();

  Serial.println("[CONFIG] AP ativo.");
  Serial.printf("[CONFIG] SSID: %s\n", AP_SSID);
  Serial.printf("[CONFIG] Senha: %s\n", AP_PASSWORD);

  Serial.print("[CONFIG] Abra: http://");
  Serial.println(WiFi.softAPIP());

  Serial.printf(
    "[MEM] Heap livre: %u bytes\n",
    ESP.getFreeHeap()
  );
}

void stopConfigMode() {
  if (!configActive) {
    return;
  }

  configActive = false;

  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_OFF);

  Serial.println("[CONFIG] Wi-Fi desligado.");
}

// ======================================================
// Botão
// ======================================================

void processButton() {
  int raw = digitalRead(PIN_BUTTON);

  if (raw != lastRawButtonState) {
    lastRawButtonState = raw;
    lastButtonChange = millis();
  }

  if (
    millis() - lastButtonChange >= DEBOUNCE_MS &&
    raw != stableButtonState
  ) {

    stableButtonState = raw;

    // Botão acabou de ser pressionado
    if (stableButtonState == LOW) {

      buttonPressStarted = millis();
      longPressHandled = false;

      if (state == DeviceState::ALARM) {
        acknowledgeAlarm();

        // Evita abrir configuração se mantiver pressionado
        longPressHandled = true;
      }
    }
  }

  // Long press em IDLE
  if (
    stableButtonState == LOW &&
    state != DeviceState::ALARM &&
    !longPressHandled &&
    millis() - buttonPressStarted >= CONFIG_LONG_PRESS_MS
  ) {

    longPressHandled = true;

    if (configActive) {
      stopConfigMode();
    } else {
      startConfigMode();
    }
  }
}

// ======================================================
// Setup
// ======================================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println(" MedicineAlarm - Day 2 + Day 3");
  Serial.println("==============================");

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_BUZZER, BUZZER_OFF);

  preferences.begin("medicine", false);

  acknowledgementCount =
    preferences.getUInt("ack_count", 0);

  Serial.printf(
    "[NVS] Reconhecimentos anteriores: %u\n",
    acknowledgementCount
  );

  loadAlarms();

  setupWebServer();

  WiFi.mode(WIFI_OFF);

  esp_task_wdt_init(8, true);
  esp_task_wdt_add(NULL);

  Serial.println("[WDT] Watchdog ativo.");

  Serial.printf(
    "[MEM] Heap livre: %u bytes\n",
    ESP.getFreeHeap()
  );

  Serial.println();
  Serial.println(
    "Segure o botao por 3 segundos para configurar."
  );

  enterIdle();
}

// ======================================================
// Loop
// ======================================================

void loop() {
  esp_task_wdt_reset();

  processButton();

  updateLed();

  if (configActive) {
    server.handleClient();

    if (
      millis() - lastConfigActivity >
      CONFIG_TIMEOUT_MS
    ) {
      Serial.println(
        "[CONFIG] Timeout. Desligando Wi-Fi."
      );

      stopConfigMode();
    }
  }

  delay(5);
}
