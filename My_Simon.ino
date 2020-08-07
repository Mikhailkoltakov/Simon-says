#define Red_LED 2
#define Green_LED 3
#define Blue_LED 4
#define Red_Button_Sens 11
#define Green_Button_Sens 12
#define Blue_Button_Sens 13
#define Buzzer 5
#define N 15 //максимальная длина последовательности


boolean Mistake = false; //Игра заканчивается, если была допущена ошибка, переменная тогда будет true
int n = 1; //задает размерность последовательности в каждом раунде
int k = 0; //номер элемента, который мы угадываем (в массиве индексы начинаются с 0)
int b = 0; //номер нажатой кнопки - 1, 2 или 3
int lamps[N]; //Массив - послеовательность сигналов ламп, красная - 1, зелёная - 2, синяя - 3
int button1; //переменные для считывания показаний с кнопок |
int button2; //                                             |
int button3; //                                             |


void setup() {
  pinMode(Red_LED, OUTPUT); //Красный светодиод
  pinMode(Green_LED, OUTPUT); //Зелёный светодиод
  pinMode(Blue_LED, OUTPUT); //Синий светодиод
  pinMode(Buzzer, OUTPUT); //Пищалка

  pinMode(Red_Button_Sens, INPUT); //Кнопка на красный светодиод
  pinMode(Green_Button_Sens, INPUT); //Кнопка на зелёный светодиод
  pinMode(Blue_Button_Sens, INPUT); //Кнопка на синий светодиод
  Serial.begin(9600);
  randomSeed(analogRead(A0)); //читаем сигнал с неподключенного аналогового пина
}

void loop() {
  delay(3000);
  while (!Mistake && (n <= N)) {
    Serial.print("n="); Serial.println(n);
    // самая первая последовательность будет из одного элемента: n=1, i=0, первый элемент - lamps[0]
    // последний: n=N, i=N-1, lamps[N-1]

    //заполняем послдовательность и мигаем соответствующими светодиодами:
    for (int i = 0; i < n; i++) {
      lamps[i] = random(1, 4); // то есть от 1 до 3 включительно
      Serial.print(lamps[i]);
      Serial.print("  ");
      switch (lamps[i]) {
        case 1:
          digitalWrite(Red_LED, HIGH);
          delay(500);
          tone(Buzzer, 1500);
          digitalWrite(Red_LED, LOW);
          noTone(Buzzer);
          delay(500);
          break;
        case 2:
          digitalWrite(Green_LED, HIGH);
          tone(Buzzer, 3000);
          delay(500);
          digitalWrite(Green_LED, LOW);
          noTone(Buzzer);
          delay(500);
          break;
        case 3:
          digitalWrite(Blue_LED, HIGH);
          tone(Buzzer, 4500);
          delay(500);
          digitalWrite(Blue_LED, LOW);
          noTone(Buzzer);
          delay(500);
          break;
      }
    }
    Serial.println();
    k = 0;
    // 1-й элемент последовательности будет lamps[0]
    while (!Mistake && (k < n)) {
      do {
        button1 = digitalRead(Red_Button_Sens);
        button2 = digitalRead(Green_Button_Sens);
        button3 = digitalRead(Blue_Button_Sens);
      } while (!button1 && !button2 && !button3);

      if button1 {
        digitalWrite(Red_LED, HIGH);
        tone(Buzzer, 1500);
        delay(200);
        noTone(Buzzer);
        digitalWrite(Red_LED, LOW);
        b = 1;
      }
      else if button2 {
        digitalWrite(Green_LED, HIGH);
        tone(Buzzer, 3000);
        delay(200);
        digitalWrite(Green_LED, LOW);
        noTone(Buzzer);
        b = 2;
      }
      else if button3 {
        digitalWrite(Blue_LED, HIGH);
        tone(Buzzer, 4500);
        delay(200);
        digitalWrite(Blue_LED, LOW);
        noTone(Buzzer);
        b = 3;
      }

      Serial.print(b);
      Serial.print("  ");

      if (b == lamps[k])  {
        k = k + 1; // самый последний элемент в игре будет lamps[N-1], затем k станет равно N
        delay(500);
      }
      else { // вы проиграли
        Serial.println("ERROR");
        Mistake = true;
        tone(Buzzer, 3000);
        delay(300);
        tone(Buzzer, 2000);
        delay(300);
        tone(Buzzer, 1000);
        delay(300);
        noTone(Buzzer);
      }
      // возвращаемся, чтобы нажать ещё одну кнопку (если ошиблись, не вернемся)
    }

    // мы нажали все кнопки в этом ряду, проверим, не последний ли это был ряд:
    if (k == N) { //значит, мы угадали самый последний элемент и выиграли
      Serial.println();
      Serial.println("OK");
      for (int i = 1; i <= 3; i++) {
        tone(Buzzer, i * 1000);
        digitalWrite(Red_LED, HIGH);
        digitalWrite(Green_LED, HIGH);
        digitalWrite(Blue_LED, HIGH);
        delay(300);
        digitalWrite(Red_LED, LOW);
        digitalWrite(Green_LED, LOW);
        digitalWrite(Blue_LED, LOW);
        noTone(Buzzer);
        delay(100);
      }
    }
    n = n + 1; // возвращаемся, увеличив длину последовательности
  }
}
