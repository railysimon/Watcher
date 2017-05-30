#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolBar>
#include <QSlider>
#include <QFileDialog>
#include <QDir>
#include <QTimer>
#include <QTransform>

class Window : public QWidget // клас графічного інтерефейсу
{
    Q_OBJECT // спеціальний бібліотечний макрос (для реалізацї сигнально-слотової моделі)

public:
    Window(QWidget *parent = 0); // конструктор
    ~Window(); // деструктор

    private:
            QLabel *picture; // віджет котрий відображає завантажені зображення
            QSlider *sizer; // повзунок для зміни розміру картинок
            QVector<QString> pictures; // масив шляхів до відкритих зображень
            QTimer *timer; // таймер
            QToolBar *navigation; // меню
            int current; // індекс поточного зображення, котре відображається

    private:
            void Layout(); // метод який реалізовує розміщення графічних елементів у вікні
            QToolBar *toolbar(QStringList actions, int actions_count); // метод, який створює обʼєекти меню

    private slots: // слоти - це методи, які виконуються по сигналу (події)
                    void ButtonClick(); // перевіряє яка кнопка натиснута
                    void Scaled(int); // змінює розмір
                    void SlideShow(); // реалізовує слайд шоу
                    void Rotate(); // поворот зображення
};

#endif // WINDOW_H
