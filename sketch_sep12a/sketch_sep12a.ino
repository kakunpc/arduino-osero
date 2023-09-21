#include <Adafruit_GFX.h>
#include "Adafruit_TFTLCD.h"
#include "TouchScreen.h"

#define XP              8
#define YP              A3
#define XM              A2
#define YM              9
#define TS_MINX         150 - 40
#define TS_MAXX         920 - 40
#define TS_MINY         120 - 40
#define TS_MAXY         940 - 40
#define LCD_CS          A3
#define LCD_CD          A2
#define LCD_WR          A1
#define LCD_RD          A0
#define LCD_RESET       A4
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF
#define MINPRESSURE     10
#define MAXPRESSURE     1000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
int ban[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int field[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void isi(int n, int iro) {
    int x,
        y;
    x = (n % 8) * 30 + 15;
    y = (n / 8) * 30 + 15;
    if (iro == 0) tft.fillCircle(x, y, 13, BLACK);
    if (iro == 2) tft.fillCircle(x, y, 13, WHITE);
}
int check(int pt, int d) {
    int x = pt % 8; 
    int y = floor(pt / 8);
    int res = 0;
    if (x == 0 && (d == -9 || d == -1 || d ==  7)) res = 1;
    if (x == 7 && (d == -7 || d ==  1 || d ==  9)) res = 1;
    if (y == 0 && (d == -9 || d == -8 || d == -7)) res = 1;
    if (y == 7 && (d ==  7 || d ==  8 || d ==  9)) res = 1;
    return res;
}
int sasu() {
    int suji[] = {0, 7, 56, 63, 18, 21, 42, 45, 2, 16, 5, 23, 40, 58, 47, 61, 3, 4, 11, 12, 19, 20, 24, 25, 26, 32, 33, 34, 29, 30, 31, 37, 38, 39, 43, 44, 51, 52, 59, 60, 1, 8, 9, 10, 17, 6, 13, 14, 15, 22, 41, 48, 49, 50, 57, 46, 53, 54, 55, 62};
    int res;
    int all = 0;
    int iro = 2;
    int turn = 0;
    int dir[] = {-9, 9, -7, 7, -1, 1, -8, 8};
    int i;
    int j;
    int pt;
    int count;
    int pt1;
    int f1;
    for (j = 0; j < 60; j++)
    {
        pt = suji[j];
        if (ban[pt] == 1)
        {
            for (i = 0; i < 8; i++)
            {
                count = 0;
                if (check(pt, dir[i]) == 0)
                {
                    pt1 = pt + dir[i];
                    f1 = 0;
                    do
                    {
                        if (ban[pt1] == iro)
                        {
                            count++;
                            if (check(pt1, dir[i]) == 0)
                            {
                                pt1 += dir[i];
                            }
                            else
                            {
                                f1 = 1;
                            }
                        }
                        else
                        {
                            f1 = 1;
                        }
                    } while (f1 == 0);
                }
                if ((count > 0) && (ban[pt1] == turn))
                {
                    all += count;
                }
            }
        }
        if (all > 0)
        {
            res = pt;
            break;
        }
    }
    return res;
}
int oku(int pt, int iro) {
    int res = 0;
    int turn = 0;
    if (iro == 0) turn = 2;
    int dir[] = {-9, -8, -7, -1, 1, 7, 8, 9};
    int tugi;
    int i;
    int count;
    if (ban[pt] == 1)
    {
        for (i = 0; i < 8; i++)
        {
            count = 0;
            tugi = pt;
            do
            {
                if (check(tugi, dir[i]) == 1) break;
                count++;
                tugi += dir[i];
            }
            while (ban[tugi] == turn);
            if ((count > 1) && (ban[tugi] == iro))
            {
                res = -1;
                tugi = pt;
                do
                {
                    ban[tugi] = iro;
                    isi(tugi, iro);
                    tugi += dir[i];
                }
                while (ban[tugi] == turn);
            }
        }
        if (res == -1) isi(pt, iro);
        return res;
    }
    return 0;
}
void setup(void)
{
    int i;
    Serial.begin(115200);
    Serial.println("start");
    tft.reset();
    uint16_t identifier = 0x9341;
    tft.begin(identifier);
    tft.fillScreen(BLACK);
    for (i = 0; i < 8; i++)
    {
        tft.fillRect(30 * i, 0, 30, 30, GREEN);
        tft.drawRect(30 * i, 0, 30, 30, WHITE);
        tft.fillRect(30 * i, 30, 30, 30, GREEN);
        tft.drawRect(30 * i, 30, 30, 30, WHITE);
        tft.fillRect(30 * i, 60, 30, 30, GREEN);
        tft.drawRect(30 * i, 60, 30, 30, WHITE);
        tft.fillRect(30 * i, 90, 30, 30, GREEN);
        tft.drawRect(30 * i, 90, 30, 30, WHITE);
        tft.fillRect(30 * i, 120, 30, 30, GREEN);
        tft.drawRect(30 * i, 120, 30, 30, WHITE);
        tft.fillRect(30 * i, 150, 30, 30, GREEN);
        tft.drawRect(30 * i, 150, 30, 30, WHITE);
        tft.fillRect(30 * i, 180, 30, 30, GREEN);
        tft.drawRect(30 * i, 180, 30, 30, WHITE);
        tft.fillRect(30 * i, 210, 30, 30, GREEN);
        tft.drawRect(30 * i, 210, 30, 30, WHITE);
    }
    tft.fillCircle(105, 105, 13, BLACK);
    tft.fillCircle(135, 105, 13, WHITE);
    tft.fillCircle(105, 135, 13, WHITE);
    tft.fillCircle(135, 135, 13, BLACK);
    tft.setCursor(0, 240);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println("You no ban.");
}
void loop()
{
    int n;
    int i;
    Point p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
        p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
        p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
        tft.fillRect(0, 240, 200, 30, BLACK);
        n = p.x / 30 + p.y / 30 * 8;
        //Serial.println(n);
        int pt = n;
        int res = oku(pt, 2);
        delay(1000);
        tft.setCursor(0, 240);
        if (res != 0)
        {
            pt = sasu();
            res = oku(pt, 0);
        }
        else
        {
            tft.println("boke!!");
            delay(1000);
            tft.fillRect(0, 240, 200, 30, BLACK);
            tft.setCursor(0, 240);
        }
        int end = -1;
        pt = 0;
        for (int i = 0; i < 64; i++)
        {
            if (ban[i] == 1) end = 0;
            if (ban[i] == 2) pt++;
        }
        if (end == 0)
        {
            tft.println("You no ban.");
        }
        else
        {
            tft.println("owari!!");
            tft.setCursor(0, 270);
            if (pt > 31)
            {
                tft.println("You Win!!");
            }
            else
            {
                tft.println("Me Win!!");
            }
        }
    }
}




