/*
 * ESP32 - Loja MQTT Teste Simples
 * * Versão simplificada para testes iniciais
 * Acende LED correspondente à cor do produto solicitado via MQTT
 * Desenvolvido para feira técnica - Integração Dev. Sistemas + Mecatrônica
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Configurações WiFi
const char* ssid = "SEU_WIFI";            // Substitua pelo seu WiFi
const char* password = "SUA_SENHA";        // Substitua pela sua senha

// Configurações MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "loja/pedido";

// Pinos dos LEDs por cor
const int LED_VERMELHO = 2;     // LED Vermelho - Caixa Vermelha
const int LED_AZUL = 4;         // LED Azul - Caixa Azul  
const int LED_VERDE = 5;       // LED Verde - Caixa Verde
const int LED_AMARELO = 18;     // LED Amarelo - Caixa Amarela
const int BUZZER_PIN = 21;      // Buzzer para feedback
const int BUTTON_PIN = 22;      // Botão para reset manual

// Objetos
WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis de controle
String ultimoPedido = "";
unsigned long tempoLedAceso = 0;
int ledAtivo = -1;
const unsigned long TEMPO_LED_LIGADO = 5000; // 5 segundos

void setup() {
  Serial.begin(115200);
  
  // Configurar pinos dos LEDs
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Inicializar todos os LEDs apagados
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  
  // Conectar WiFi
  conectarWiFi();
  
  // Configurar MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callbackMQTT);
  
  // Sinal de inicialização
  sequenciaInicializacao();
  
  Serial.println("Sistema de teste pronto para receber pedidos!");
  Serial.println("LEDs: Vermelho=2, Azul=4, Verde=5, Amarelo=18");
}

void loop() {
  // Manter conexão MQTT
  if (!client.connected()) {
    reconectarMQTT();
  }
  client.loop();
  
  // Verificar se deve apagar LED após tempo determinado
  if (ledAtivo != -1 && millis() - tempoLedAceso > TEMPO_LED_LIGADO) {
    apagarTodosLeds();
    ledAtivo = -1;
    Serial.println("LED apagado automaticamente");
  }
  
  // Verificar botão de reset
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      resetarSistema();
      while(digitalRead(BUTTON_PIN) == LOW); // Aguardar soltar
    }
  }
  
  delay(100);
}

void conectarWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println(" conectado!");
      client.subscribe(mqtt_topic);
      Serial.println("Inscrito no tópico: " + String(mqtt_topic));
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  // Converter payload para string
  String mensagem = "";
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  
  Serial.println("Mensagem MQTT recebida: " + mensagem);
  
  // Parse JSON
  // Alterado de DynamicJsonDocument para JsonDocument para evitar o aviso de 'deprecated'
  JsonDocument doc; 
  deserializeJson(doc, mensagem);
  
  String pedidoId = doc["pedidoId"];
  String produto = doc["produto"];
  int posicao = doc["posicao"];
  String cliente = doc["cliente"];
  
  // Verificar se é um novo pedido
  if (pedidoId != ultimoPedido) {
    ultimoPedido = pedidoId;
    processarPedidoTeste(produto, cliente, pedidoId);
  }
}

void processarPedidoTeste(String produto, String cliente, String pedidoId) {
  Serial.println("=== NOVO PEDIDO - TESTE ===");
  Serial.println("ID: " + pedidoId);
  Serial.println("Produto: " + produto);
  Serial.println("Cliente: " + cliente);
  Serial.println("============================");
  
  // Apagar todos os LEDs primeiro
  apagarTodosLeds();
  
  // Sinalizar recebimento do pedido
  tocarBuzzer(1, 300);
  
  // Acender LED correspondente à cor do produto
  if (produto.indexOf("Vermelha") != -1) {
    digitalWrite(LED_VERMELHO, HIGH);
    ledAtivo = LED_VERMELHO;
    Serial.println("🔴 LED VERMELHO aceso - Caixa Vermelha solicitada!");
  }
  else if (produto.indexOf("Azul") != -1) {
    digitalWrite(LED_AZUL, HIGH);
    ledAtivo = LED_AZUL;
    Serial.println("🔵 LED AZUL aceso - Caixa Azul solicitada!");
  }
  else if (produto.indexOf("Verde") != -1) {
    digitalWrite(LED_VERDE, HIGH);
    ledAtivo = LED_VERDE;
    Serial.println("🟢 LED VERDE aceso - Caixa Verde solicitada!");
  }
  else if (produto.indexOf("Amarela") != -1) {
    digitalWrite(LED_AMARELO, HIGH);
    ledAtivo = LED_AMARELO;
    Serial.println("🟡 LED AMARELO aceso - Caixa Amarela solicitada!");
  }
  else {
    Serial.println("❌ Produto não reconhecido: " + produto);
    tocarBuzzer(3, 100); // Sinal de erro
    return;
  }
  
  // Marcar tempo que o LED foi aceso
  tempoLedAceso = millis();
  
  Serial.println("LED ficará aceso por " + String(TEMPO_LED_LIGADO/1000) + " segundos");
}

void apagarTodosLeds() {
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  ledAtivo = -1;
}

void resetarSistema() {
  Serial.println("🔄 Resetando sistema...");
  
  ultimoPedido = "";
  
  // Apagar todos os LEDs
  apagarTodosLeds();
  
  // Sinal de reset
  tocarBuzzer(1, 500);
  
  Serial.println("✅ Sistema resetado! Pronto para novos pedidos.");
}

void sequenciaInicializacao() {
  Serial.println("🚀 Inicializando sistema de teste...");
  
  // Teste dos LEDs em sequência
  Serial.println("Testando LEDs...");
  
  digitalWrite(LED_VERMELHO, HIGH);
  Serial.println("🔴 LED Vermelho");
  delay(300);
  digitalWrite(LED_VERMELHO, LOW);
  
  digitalWrite(LED_AZUL, HIGH);
  Serial.println("🔵 LED Azul");
  delay(300);
  digitalWrite(LED_AZUL, LOW);
  
  digitalWrite(LED_VERDE, HIGH);
  Serial.println("🟢 LED Verde");
  delay(300);
  digitalWrite(LED_VERDE, LOW);
  
  digitalWrite(LED_AMARELO, HIGH);
  Serial.println("🟡 LED Amarelo");
  delay(300);
  digitalWrite(LED_AMARELO, LOW);
  
  delay(500);
  
  // Som de inicialização
  tocarBuzzer(2, 150);
  
  Serial.println("✅ Teste de LEDs concluído!");
}

void testeManualLeds() {
  Serial.println("🧪 Iniciando teste manual dos LEDs...");
  
  // Piscar todos os LEDs 3 vezes
  for(int j = 0; j < 3; j++) {
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_AZUL, HIGH);
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARELO, HIGH);
    delay(200);
    
    apagarTodosLeds();
    delay(200);
  }
  
  Serial.println("✅ Teste manual concluído!");
}

void tocarBuzzer(int vezes, int duracao) {
  for(int i = 0; i < vezes; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duracao);
    digitalWrite(BUZZER_PIN, LOW);
    if(i < vezes - 1) delay(duracao);
  }
}

/*
 * INSTRUÇÕES DE MONTAGEM PARA TESTE:
 * * 1. Conecte LED VERMELHO no pino 2 (com resistor de 220Ω)
 * 2. Conecte LED AZUL no pino 4 (com resistor de 220Ω)
 * 3. Conecte LED VERDE no pino 5 (com resistor de 220Ω)
 * 4. Conecte LED AMARELO no pino 18 (com resistor de 220Ω)
 * 5. Conecte buzzer no pino 21 (opcional)
 * 6. Conecte botão no pino 22 (com pull-up interno)
 * 7. Configure seu WiFi nas variáveis ssid e password
 * 8. Carregue o código no ESP32
 * * FUNCIONAMENTO DO TESTE:
 * - Quando receber pedido via MQTT, acende LED da cor correspondente
 * - Caixa Vermelha → LED Vermelho (pino 2)
 * - Caixa Azul → LED Azul (pino 4)
 * - Caixa Verde → LED Verde (pino 5)
 * - Caixa Amarela → LED Amarelo (pino 18)
 * - LED fica aceso por 5 segundos e apaga automaticamente
 * - Buzzer dá feedback sonoro quando recebe pedido
 * - Botão permite reset manual do sistema
 * * TESTE:
 * 1. Faça um pedido no e-commerce
 * 2. Observe o LED correspondente acender
 * 3. Verifique as mensagens no Serial Monitor
 * 4. Aguarde 5 segundos para o LED apagar automaticamente
 */
