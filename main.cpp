#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int SensorTurbidez = 34;
const int oneWireBus = 32;


int i;
float voltagem;
float NTU;
int leitura_analogica;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
float tempAgua;

void  get_values_st100();
void  get_values_ds18b20();
void  mostra_valores();

void setup(){

  Serial.begin(115200);
  sensors.begin();

}

void loop(){

  get_values_st100();
  get_values_ds18b20();
  mostra_valores();

  delay(3000);
}

void get_values_ds18b20(){
    sensors.requestTemperatures(); 
    // Temperature in Celsius degrees
    tempAgua = sensors.getTempCByIndex(0);
    
}


float ArredondarPara( float ValorEntrada, int CasaDecimal ) {
  float multiplicador = powf( 10.0f, CasaDecimal );
  ValorEntrada = roundf( ValorEntrada * multiplicador ) / multiplicador;
  return ValorEntrada;
}


void get_values_st100(){
  voltagem = 0;
  for (i = 0; i < 800; i++) {
    voltagem += ((float)analogRead(SensorTurbidez) / 1023) * 5;
  }
  
  // Realiza a média entre os valores lidos na função for acima
    voltagem = voltagem / 800;
    voltagem = ArredondarPara(voltagem, 1);
  
  // Se Voltagem menor que 2.5 fixa o valor de NTU
  if (voltagem < 2.5) {
    NTU = 3000;
  }
  
  else if (voltagem > 4.2) {
    NTU = 0;
    voltagem = 4.2;
  }
  
  // Senão calcula o valor de NTU através da fórmula
  else {
    NTU = -1120.4 * sqrt(voltagem) + 5742.3 * voltagem - 4353.8;
  }

}

void mostra_valores(){
  Serial.print("Leitura do sensor de turbidez: ");
  Serial.print(voltagem);
  Serial.println(" V");
  Serial.print(NTU);
  Serial.println(" NTU");
  Serial.print("--------------------------------");
  Serial.print("Leitura do sensor de temperatura: ");
  Serial.println(tempAgua);

}