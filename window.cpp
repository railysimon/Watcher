#include "window.h"

Window::Window(QWidget *parent) : QWidget(parent), current(0)
{
    Layout();

    timer = new QTimer(this);
    timer->setInterval(2000);
    connect(timer, SIGNAL(timeout()), this, SLOT(SlideShow()));
}

Window::~Window()
{

}

void Window::Layout()
{
    picture = new QLabel; // створення обʼєкта віджета, який відображає зображення
    picture->setAlignment(Qt::AlignCenter); // орієнтація посередині
    picture->setStyleSheet("background: rgb(36, 37, 38);"); // КСС стиль

    QAction *popup = new QAction("Rotate", picture); // контексне меню, по натисканю правої кнопки мишки
    picture->addAction(popup);
    picture->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(popup, SIGNAL(triggered(bool)), this, SLOT(Rotate()));

    QStringList names;
    names << "close" << "full" << "open";

    QHBoxLayout *menu_layout = new QHBoxLayout;
    menu_layout->setAlignment(Qt::AlignLeft);

    QHBoxLayout *first_menu_layout  = new QHBoxLayout;
    first_menu_layout->addWidget(toolbar(names, 3)); // створення першого меню (опен, фуллскрін..) і добавлення на форму

    names.clear();
    names << "previous" << "slide" << "next";

    QHBoxLayout *second_menu_layout  = new QHBoxLayout;
    second_menu_layout->setAlignment(Qt::AlignCenter);
    navigation = toolbar(names, 3); // створення другого меню (переключателі, слайдшоу)
    navigation->setEnabled(false);
    second_menu_layout->addWidget(navigation);

    menu_layout->addLayout(first_menu_layout);
    menu_layout->addSpacing(90);
    menu_layout->addLayout(second_menu_layout);
    menu_layout->addSpacing(50);

    sizer = new QSlider(Qt::Horizontal); // створення повзунка
    sizer->setMinimum(40);
    sizer->setMaximum(120);
    sizer->setSingleStep(20);

    sizer->setMaximumWidth(200);
    sizer->setEnabled(false);
    connect(sizer, SIGNAL(valueChanged(int)), this, SLOT(Scaled(int)));
    menu_layout->addWidget(sizer);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(picture);
    main_layout->addLayout(menu_layout);
    this->setLayout(main_layout);

    this->move(qApp->desktop()->width() / 2 - width()/2, qApp->desktop()->height() / 2 - height()/2); // розміщення вінка
    // посередині екрану
    this->setMinimumSize(600, 500);
    this->setStyleSheet("background: rgb(66, 67, 68);"); // стиль вікна (КСС)
}

QToolBar *Window::toolbar(QStringList actions, int actions_count) // метод створення меню
{
    QToolBar *bar = new QToolBar;

    bar->setIconSize(QSize(30, 30));
    bar->setMaximumWidth(150);
    bar->setStyleSheet("color: white;");

    for(int i=0; i<actions_count; i++)
        bar->addAction(QIcon(QPixmap(QApplication::applicationDirPath() + "/" +
                                     actions[i])), actions[i], this, SLOT(ButtonClick()));

    return bar;
}

void Window::ButtonClick()
{
    QAction *action = qobject_cast<QAction*> (sender());

    if(action->text() == "open") // якщо натиснута кнопка "відкрити"
    {
        QString path = QFileDialog::getExistingDirectory(this, "Open...", "");

        if(!path.isEmpty())
        {
            QDir dir(path);
            QString filter = "*.jpg *.png *.jpeg";
            QStringList pixes = dir.entryList(filter.split(" "), QDir::Files); // считуємо картинки, які є в папці по
            // заданих фільтрах

            for(int i=0; i<pixes.size(); i++)
                pictures.push_back(dir.absoluteFilePath((pixes[i])));

            picture->setPixmap(QPixmap(pictures[current])); // встановлення першої картинки із завантажених
            sizer->setEnabled(true);
            navigation->setEnabled(true);

        }
        else
        {
            sizer->setEnabled(false);
            navigation->setEnabled(false);
        }
    }

    else if(action->text() == "full") // якщо натиснуна кнопка на повний екран
    {
        static bool full = false;
        full = !full;

        if(full) this->showFullScreen();
        else this->showNormal();
    }

    else if(action->text() == "close") // якщо натиснута кнопка закрити вікно
        qApp->quit();

    else if(action->text() == "previous") // попереднє зображення
    {
        current--;
        if(current < 0) current = pictures.size() - 1;

        picture->setPixmap(QPixmap(pictures[current]));
    }

    else if(action->text() == "next") // наступне зображення
    {
        current++;
        if(current > pictures.size() - 1) current = 0;

        picture->setPixmap(QPixmap(pictures[current]));
    }

    else if(action->text() == "slide") // слайдшоу
    {
        static bool slide = false;
        slide = !slide;

        action->setCheckable(true);

        if(slide)
        {
            timer->start();
            action->setChecked(true);
        }
        else
        {
            timer->stop();
            action->setChecked(false);
        }
    }
}

void Window::Scaled(int x) // слот для зміни розміру
{
    QPixmap temp(pictures[current]);

    temp = temp.scaled(temp.size() * (x / 20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    picture->setPixmap(temp);
}

void Window::SlideShow() // слайдшоу
{
    current++;
    if(current > pictures.size() - 1) current = 0;

    picture->setPixmap(QPixmap(pictures[current]));
}

void Window::Rotate() // поворот
{
    static int angle = 90;
    QTransform transform;

    if(!pictures.isEmpty())
    {
        transform.rotate(angle);
        angle += 90;

        if(angle > 360) angle = 90;

        QPixmap temp(pictures[current]);
        temp = temp.transformed(transform);
        picture->setPixmap(temp);
    }
}
