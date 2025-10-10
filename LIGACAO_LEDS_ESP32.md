# 🔌 Guia de Ligação dos LEDs no ESP32

## 📋 Materiais Necessários
- 1x ESP32 DevKit
- 1x Protoboard
- 4x LEDs (Vermelho, Azul, Verde, Amarelo)
- 4x Resistores de 220Ω
- Jumpers macho-macho
- 1x Buzzer (opcional)
- 1x Botão push-button

## 🎯 Mapeamento dos Pinos

Segundo o código ESP32, os pinos utilizados são:

| Componente | Pino ESP32 | Cor do LED |
|------------|------------|------------|
| LED Vermelho | GPIO 2 | 🔴 Vermelho |
| LED Azul | GPIO 4 | 🔵 Azul |
| LED Verde | GPIO 5 | 🟢 Verde |
| LED Amarelo | GPIO 18 | 🟡 Amarelo |
| Buzzer | GPIO 21 | - |
| Botão Reset | GPIO 22 | - |

## 🔧 Instruções de Ligação

### 1. Alimentação
- **GND do ESP32** → **Trilha negativa da protoboard (-)**
- **3.3V do ESP32** → **Trilha positiva da protoboard (+)**

### 2. LEDs com Resistores

Cada LED deve ser ligado da seguinte forma:

#### LED Vermelho (GPIO 2)
```
ESP32 GPIO 2 → Resistor 220Ω → Anodo do LED (+) → Catodo do LED (-) → GND
```

#### LED Azul (GPIO 4)
```
ESP32 GPIO 4 → Resistor 220Ω → Anodo do LED (+) → Catodo do LED (-) → GND
```

#### LED Verde (GPIO 5)
```
ESP32 GPIO 5 → Resistor 220Ω → Anodo do LED (+) → Catodo do LED (-) → GND
```

#### LED Amarelo (GPIO 18)
```
ESP32 GPIO 18 → Resistor 220Ω → Anodo do LED (+) → Catodo do LED (-) → GND
```

### 3. Buzzer (Opcional)
```
ESP32 GPIO 21 → Pino positivo do Buzzer
GND → Pino negativo do Buzzer
```

### 4. Botão Reset
```
ESP32 GPIO 22 → Um terminal do botão
GND → Outro terminal do botão
```
*Nota: O código usa pull-up interno, não precisa de resistor externo*

## 📐 Layout Sugerido na Protoboard

Baseado na sua imagem, sugiro o seguinte layout:

```
     ESP32
   ┌─────────┐
   │         │
   │    2    │ ──── Resistor 220Ω ──── LED Vermelho ──── GND
   │    4    │ ──── Resistor 220Ω ──── LED Azul ──── GND
   │    5    │ ──── Resistor 220Ω ──── LED Verde ──── GND
   │   18    │ ──── Resistor 220Ω ──── LED Amarelo ──── GND
   │   21    │ ──── Buzzer (+) ──── Buzzer (-) ──── GND
   │   22    │ ──── Botão ──── GND
   │   GND   │ ──── Trilha negativa (-)
   │  3.3V   │ ──── Trilha positiva (+)
   └─────────┘
```

## ⚡ Passos de Montagem

### Passo 1: Posicionar o ESP32
1. Coloque o ESP32 no centro da protoboard
2. Certifique-se que os pinos estejam bem encaixados

### Passo 2: Conectar Alimentação
1. Ligue o GND do ESP32 à trilha negativa (-)
2. Ligue o 3.3V do ESP32 à trilha positiva (+)

### Passo 3: Montar os LEDs
1. **LED Vermelho:**
   - Coloque o resistor entre GPIO 2 e uma linha da protoboard
   - Conecte o anodo (perna longa) do LED vermelho após o resistor
   - Conecte o catodo (perna curta) ao GND

2. **LED Azul:**
   - Coloque o resistor entre GPIO 4 e uma linha da protoboard
   - Conecte o anodo do LED azul após o resistor
   - Conecte o catodo ao GND

3. **LED Verde:**
   - Coloque o resistor entre GPIO 5 e uma linha da protoboard
   - Conecte o anodo do LED verde após o resistor
   - Conecte o catodo ao GND

4. **LED Amarelo:**
   - Coloque o resistor entre GPIO 18 e uma linha da protoboard
   - Conecte o anodo do LED amarelo após o resistor
   - Conecte o catodo ao GND

### Passo 4: Buzzer (Opcional)
1. Conecte o pino positivo do buzzer ao GPIO 21
2. Conecte o pino negativo do buzzer ao GND

### Passo 5: Botão Reset
1. Coloque o botão na protoboard
2. Conecte um terminal ao GPIO 22
3. Conecte o outro terminal ao GND

## ✅ Verificação

Antes de ligar:
1. ✓ Todos os LEDs têm resistores de 220Ω
2. ✓ Polaridade dos LEDs está correta (anodo +, catodo -)
3. ✓ Todas as conexões GND estão firmes
4. ✓ Não há curto-circuitos
5. ✓ ESP32 está bem encaixado

## 🧪 Teste

1. Carregue o código no ESP32
2. Abra o Serial Monitor (115200 baud)
3. Observe a sequência de teste dos LEDs na inicialização
4. Faça um pedido no e-commerce
5. Verifique se o LED correspondente acende

## ⚠️ Dicas Importantes

- **Polaridade:** LEDs só funcionam em uma direção
- **Resistores:** Sempre use resistores para proteger os LEDs
- **Conexões:** Certifique-se que estão firmes
- **Alimentação:** Use 3.3V, não 5V para os LEDs
- **Teste:** Teste cada LED individualmente se houver problemas

## 🔍 Troubleshooting

| Problema | Possível Causa | Solução |
|----------|----------------|----------|
| LED não acende | Polaridade invertida | Inverta as conexões do LED |
| LED muito fraco | Resistor muito alto | Use resistor de 220Ω |
| LED queima | Sem resistor | Sempre use resistor |
| Nenhum LED funciona | Problema no GND | Verifique conexão GND |
| ESP32 não conecta WiFi | Código não carregado | Recarregue o código |

---

**🎯 Objetivo:** Quando fizer um pedido no e-commerce, o LED da cor correspondente deve acender por 5 segundos!

- Caixa Vermelha → LED Vermelho 🔴
- Caixa Azul → LED Azul 🔵
- Caixa Verde → LED Verde 🟢
- Caixa Amarela → LED Amarelo 🟡