#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"
#include <avr/io.h>
#include <avr/interrupt.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 scale;
const int DT_PIN  = 2;
const int SCK_PIN = 3;
#define BTN_CLEAR_BIT    (1 << 4)
#define BTN_VIEW_BIT     (1 << 5)
#define BTN_SCROLL_BIT   (1 << 6)
#define BTN_CONFIRM_BIT  (1 << 7)
#define LED_BIT          (1 << 0)
#define LED_ON()               (PORTB |=  LED_BIT)
#define LED_OFF()              (PORTB &= ~LED_BIT)
#define BTN_CLEAR_PRESSED()    (!(PIND & BTN_CLEAR_BIT))
#define BTN_VIEW_PRESSED()     (!(PIND & BTN_VIEW_BIT))
#define BTN_SCROLL_PRESSED()   (!(PIND & BTN_SCROLL_BIT))
#define BTN_CONFIRM_PRESSED()  (!(PIND & BTN_CONFIRM_BIT))
volatile uint32_t ms_ticks = 0;
ISR(TIMER1_COMPA_vect) {
    ms_ticks++;
}
void timer1_init() {
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    OCR1A  = 249;
    TIMSK1 |= (1 << OCIE1A);
    sei();
}
void delay_ms(uint32_t ms) {
    uint32_t start = ms_ticks;
    while ((ms_ticks - start) < ms);
}
void gpio_init() {
    DDRD  &= ~(BTN_CLEAR_BIT|BTN_VIEW_BIT|BTN_SCROLL_BIT|BTN_CONFIRM_BIT);
    PORTD |=  (BTN_CLEAR_BIT|BTN_VIEW_BIT|BTN_SCROLL_BIT|BTN_CONFIRM_BIT);
    DDRB  |=  LED_BIT;
    PORTB &= ~LED_BIT;
}
typedef struct {
    char  name[10];
    float cal_per_g;
    float pro_per_g;
} FoodItem;
const int MENU_SIZE = 8;
FoodItem menu[MENU_SIZE] = {
    {"Chicken",  1.65, 0.31},
    {"Rice",     1.30, 0.03},
    {"Eggs",     1.55, 0.13},
    {"Beef",     2.50, 0.26},
    {"Broccoli", 0.34, 0.03},
    {"Oats",     3.89, 0.17},
    {"Whey Pro", 3.70, 0.78},
    {"Salmon",   2.08, 0.20}
};
int   menu_idx       = 0;
float daily_cal      = 0;
float daily_pro      = 0;
float current_weight = 0;
void setup() {
    gpio_init();
    timer1_init();
    lcd.init();
    lcd.backlight();
    lcd.print("System Booting..");
    scale.begin(DT_PIN, SCK_PIN);
    scale.set_scale(4.2);
    scale.tare();
    delay_ms(1000);
    lcd.clear();
}
void loop() {
    current_weight = scale.get_units(3);
    if (current_weight < 0) current_weight = 0;
    lcd.setCursor(0, 0);
    lcd.print("Food: ");
    lcd.print(menu[menu_idx].name);
    lcd.print("       ");
    lcd.setCursor(0, 1);
    lcd.print("Wt: ");
    lcd.print(current_weight, 0);
    lcd.print("g        ");
    if (BTN_CLEAR_PRESSED()) {
        LED_ON();
        scale.tare();
        lcd.clear();
        lcd.print("Scale Cleared!");
        delay_ms(1000);
        LED_OFF();
        lcd.clear();
    }
    if (BTN_VIEW_PRESSED()) {
        LED_ON();
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("Total C: "); lcd.print(daily_cal,0);
        lcd.setCursor(0,1); lcd.print("Total P: "); lcd.print(daily_pro,0);
        uint32_t hold_timer = 0; bool did_reset = false;
        while (BTN_VIEW_PRESSED()) {
            delay_ms(100); hold_timer += 100;
            if (hold_timer >= 2000) {
                lcd.clear(); lcd.setCursor(0,0);
                lcd.print("Totals Reset!");
                daily_cal = 0; daily_pro = 0;
                did_reset = true; delay_ms(1500); break;
            }
        }
        if (!did_reset) { delay_ms(4000); }
        LED_OFF(); lcd.clear();
    }
    if (BTN_SCROLL_PRESSED()) {
        LED_ON();
        menu_idx++;
        if (menu_idx >= MENU_SIZE) menu_idx = 0;
        delay_ms(300);
        LED_OFF();
    }
    if (BTN_CONFIRM_PRESSED()) {
        LED_ON();
        float added_cal = current_weight * menu[menu_idx].cal_per_g;
        float added_pro = current_weight * menu[menu_idx].pro_per_g;
        daily_cal += added_cal; daily_pro += added_pro;
        lcd.clear(); lcd.setCursor(0,0); lcd.print("Saved!");
        lcd.setCursor(0,1); lcd.print("C:"); lcd.print(daily_cal,0);
        lcd.print(" P:"); lcd.print(daily_pro,0);
        delay_ms(4000); LED_OFF(); lcd.clear();
    }
}