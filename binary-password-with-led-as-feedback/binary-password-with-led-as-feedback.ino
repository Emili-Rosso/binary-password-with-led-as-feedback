int botao1 = 6;
int botao2 = 7;

int LED1 = 2;
int LED2 = 3;
int LED3 = 4;
int LED4 = 5;

  
int leds[4] = {LED1, LED2, LED3, LED4};

int entrada[4];
int indice = 0; //inicia em zero
int estado = 0; //indica ESPERA (0), SUCESSO (1) ou ERRO (2)

unsigned long tempoInicio = 0;
unsigned long tempoPisca = 0;

int ledsLigados = 0;
int piscadas = 0;

int botao1Anterior = HIGH;
int botao2Anterior = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);
}

void loop() {
  
  unsigned long agora = millis();

  int leituraBotao1 = digitalRead(botao1);
  int leituraBotao2 = digitalRead(botao2);

  //Esperando 
  if (estado == 0) {

    if (botao1Anterior == HIGH && leituraBotao1 == LOW) {
      entrada[indice] = 0; //bit 0
      indice = indice + 1; //soma uma unidade ao índice
    }

    if (botao2Anterior == HIGH && leituraBotao2 == LOW) {
      entrada[indice] = 1; //bit 1
      indice = indice + 1; //soma uma unidade ao índice
    }

    if (indice == 4) {

      int correta;

      if (entrada[0] == 1 && entrada[1] == 0 && entrada[2] == 1 && entrada[3] == 0) {
        correta = 1;
      } else {
        correta = 0;
      }

      if (correta == 1) {
        estado = 1; //SUCESSO
        tempoInicio = agora;
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
      } else {
        estado = 2; //ERRO
        piscadas = 0;
        ledsLigados = 0;
        tempoPisca = agora;
      }

      indice = 0;
    }
  }

  //Estado sucesso
  if (estado == 1) {
    if (agora - tempoInicio >= 2000) {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      estado = 0;
    }
  }

  //Estado erro
  if (estado == 2) {
    if (agora - tempoPisca >= 150) {
      tempoPisca = agora;

      if (ledsLigados == 0) {
        ledsLigados = 1;
      } else {
        ledsLigados = 0;
        piscadas = piscadas + 1;
      }

      if (ledsLigados == 1) {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
        digitalWrite(LED4, HIGH);
      } else {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        digitalWrite(LED4, LOW);
      }

      if (piscadas == 3) {
        estado = 0;
      }
    }
  }

  botao1Anterior = leituraBotao1;
  botao2Anterior = leituraBotao2;
}

