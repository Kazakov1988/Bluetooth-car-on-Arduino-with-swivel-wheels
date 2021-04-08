/* Машинка на Arduino с дистанционным управлением по Bluetooth с шасси с поворотными колёсами.
   Разработанно на основе Remote XY https://remotexy.com/ 
   Ссылка на проект Remote XY - https://remotexy.com/ru/editor/b60158c4a06ce459750aa2cfed7d6035/
   
   Для компиляции понадобится библиоетки:
   RemoteXY.h - https://remotexy.com/ru/library/
   Servo.h - https://github.com/arduino-libraries/Servo
   
   22.03.2021 
*/

#define REMOTEXY_MODE__HARDSERIAL             // Определение режима соединения и подключение библиотеки RemoteXY.
#include <RemoteXY.h>

#define REMOTEXY_SERIAL Serial                // Настройки соединения. 
#define REMOTEXY_SERIAL_SPEED 9600

#pragma pack(push, 1)                         // Конфигурация интерфейса.
uint8_t RemoteXY_CONF[] =
  { 255,4,0,0,0,43,0,10,13,0,
  4,176,-6,46,71,14,2,26,1,1,
  82,2,16,20,2,31,226,134,145,0,
  1,1,82,41,16,20,2,31,226,134,
  147,0,3,131,11,9,32,12,2,26 };

struct {                                     // Структура определяет все переменные и события интерфейса управления.
  int8_t rudder;                             // Руль: -100 - 100 положение слайдера.
  uint8_t forward;                           // Вперёд: 1 если кнопка нажата, иначе 0.
  uint8_t back;                              // Назад: 1 если кнопка нажата, иначе 0.
  uint8_t speed;                             // Скорость: если переключатель в положении A равно 0, если в положении B равно 1,если в положении C равно 2.

  uint8_t connect_flag;                      // 1 если есть подключение, иначе 0.

}

RemoteXY;
#pragma pack(pop)

#define PIN_FORWARD 2                        // Пины подключения L298N к Arduino. 
#define PIN_BACK 4
#define PIN_SPEED 3

#include <Servo.h>
Servo servoRudder;
int deg;                                     // Переменная угла поворота руля.

void setup() {
  RemoteXY_Init ();

  pinMode (PIN_FORWARD, OUTPUT);
  pinMode (PIN_BACK, OUTPUT);
  pinMode (PIN_SPEED, OUTPUT);

  servoRudder.attach(5);                     // Пин подключения серво-мотора руля. 
  RemoteXY.rudder = 0;    
}

void loop() {
  RemoteXY_Handler ();
  
  if (RemoteXY.speed == 0) {                                         // Управление скоростью.
    analogWrite (PIN_SPEED, 75);
  }
  if (RemoteXY.speed == 1) {
    analogWrite (PIN_SPEED, 90);
  }
  if (RemoteXY.speed == 2) {
    analogWrite (PIN_SPEED, 250);
  }

  digitalWrite(PIN_FORWARD, (RemoteXY.forward == 0) ? LOW : HIGH);    // Назад и в перёд.
  digitalWrite(PIN_BACK, (RemoteXY.back == 0) ? LOW : HIGH);

  deg = RemoteXY.rudder;                                              // Управление рулём: 135 и 35 - это крайние левое и праволе положения. 
  deg = map (deg, -100, 100, 135, 35);
  servoRudder.write (deg);
}
