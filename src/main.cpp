//https://easytromlabs.com/arduino/arduino-lab-19-leitura-e-datalogger-de-temperatura-utilizando-um-termopar-tipo-k-e-o-max6675/
#include "main.h"

//ver https://provideyourown.com/2011/analogwrite-convert-pwm-to-voltage/
//    https://www.instructables.com/id/Analog-Output-Convert-PWM-to-Voltage/

void setup()
{
    Serial.begin(9600);
    pinMode(CS1, OUTPUT);
    pinMode(SCLK, OUTPUT);
    pinMode(SO, INPUT);
    pinMode(TUBE, OUTPUT);
    digitalWrite(CS1, HIGH);
    lcd.begin(16, 2);
    lcd.clear();
    lcd.backlight();
}

void loop()
{
    sensor1 = readThermocouple(CS1);
    Serial.println(sensor1);
    lcd_display();
    update_tube();
    delay(1000);
}

void update_tube()
{
    int temperatureVoltage = map(sensor1, MIN_TEMP,MAX_TEMP, 0, 255); 
    analogWrite(TUBE,temperatureVoltage);

}

void lcd_display()
{
    lcd.setCursor(0, 0);
    //lcd.write(1);
    lcd.setCursor(2, 0);
    lcd.write("          ");
    lcd.setCursor(2, 0);
    lcd.print(sensor1);
    lcd.write(0b11011111);
    lcd.print("C");
}

double readThermocouple(int ChipSelect)
{
    uint16_t v; // Declara variável de 16 bits - Tamanho ideal para o dado entregue pelo MAX

    digitalWrite(ChipSelect, LOW);
    _delay_ms(1);
    v = SPIRead();  // Leitura da parte alta - Primeiros 8 bits de dados
    v <<= 8;        // Desloca 8 posições para a esquerda
    v |= SPIRead(); // Faz leitura da parte baixo e armazena nos 8 primeiros bits atravez de uma OR
    digitalWrite(ChipSelect, HIGH);

    // Confere o bit 2 para termopar acoplado ou não
    if (v & 0x4)
    {
        return NAN;
    }
    v >>= 3;         // Descarta os 3 primeiros bits, onde apenas o bit 2 carrega o status e lê do bit 3 ao 14
    return v * 0.25; // Multiplica pela resolução do MAX
}

/**************************************************************************************/
// Função que implementa um "SPI" via Software para leitura dos dados no MAX6675
/**************************************************************************************/
byte SPIRead(void)
{
    int i;
    byte d = 0;

    // Laço FOR para apanhar 8 bits de cada vez
    for (i = 7; i >= 0; i--)
    {
        digitalWrite(SCLK, LOW); // Borda de descida do clock - Falling Edge
        _delay_ms(1);            // Aguarda 1ms
        if (digitalRead(SO))
        {                         // Faz a leitura do pino de dados de saída do MAX
            d |= (1 << i);        // Armazena os bits lidos, 0 ou 1, na variável e desloca de acordo com o indice para
        }                         // preencher todo o byte de dados
        digitalWrite(SCLK, HIGH); // Borda de subida do clock - Rising Edge
        _delay_ms(1);             // Aguarda 1ms
    }
    return d; // Retirna o byte de leitura dos dados lidos do MAX
}