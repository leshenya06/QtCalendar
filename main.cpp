#include <QApplication>
#include <QCalendarWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>
#include <QMouseEvent>
#include <QInputDialog>
#include <QMap>
#include <QStringList>
#include <QLabel>
#include <QHeaderView>
#include <QToolButton>


class CalendarExample : public QCalendarWidget {
    Q_OBJECT

public:

    CalendarExample(QWidget *parent = nullptr) : QCalendarWidget(parent) {
        // Устанавливаем размер ячеек
        setMinimumSize(1600, 800);

        // Подключаем сигнал clicked к слоту
        connect(this, &QCalendarWidget::clicked, this, &CalendarExample::onDateClicked);

    }

private slots:
    void onDateClicked(const QDate &date) {
        // Обновляем метку с выбранной датой
        dateLabel->setText("Вы выбрали: " + date.toString());

        // Получаем дату, на которую нажали
        QDate _date = selectedDate();
        // Запрашиваем задачу у пользователя
        bool ok;
        QString task = QInputDialog::getText(this, tr("Добавить задачу"),
                                              tr("Задача:"), QLineEdit::Normal,
                                              QString(), &ok);
        if (ok && !task.isEmpty()) {
            addTask(_date, task); // Добавляем задачу
        }
    }

protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const override {
            // Изменяем цвет выделения для текущей даты
            if (date == selectedDate()) {
                painter->fillRect(rect, QColor(242, 230, 242)); // Цвет выделения
            }

            // Рисуем фон ячейки
            painter->save();
            painter->setPen(Qt::black); // Устанавливаем цвет текста

            // Корректируем прямоугольник для текста
            QRect textRect = rect.adjusted(0, 10, -10, 0); // Отступ сверху
            painter->drawText(textRect, Qt::AlignRight | Qt::AlignTop, QString::number(date.day())); // Выравнивание по правому верхнему углу
            painter->restore();
            // Рисуем задачи, если они есть для данной даты
            if (tasks.contains(date)) {
                painter->save();
                painter->drawText(rect, Qt::AlignBottom | Qt::AlignLeft, tasks[date].join("\n ")); // Рисуем задачи
                painter->restore();
            }
        }

public slots:
    void addTask(const QDate &date, const QString &task) {
        tasks[date].append(task); // Добавляем задачу в список задач для данной даты
        updateCell(date); // Обновляем ячейку
    }

    void setDateLabel(QLabel *label) {
        dateLabel = label; // Устанавливаем метку для отображения выбранной даты
    }

private:
    QMap<QDate, QStringList> tasks; // Хранит задачи для каждой даты
    QLabel *dateLabel; // Метка для отображения выбранной даты
};

class CalendarApp : public QWidget {
    Q_OBJECT

public:
    CalendarApp(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        CalendarExample *calendar = new CalendarExample(this);
        calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
        calendar->setGridVisible(true);
        calendar->setStyleSheet(
                "QCalendarWidget {"
                                "   font-family: 'Montserrat';" // Устанавливаем шрифт
                                "}"
                "QCalendarWidget QWidget#qt_calendar_navigationbar {"
                "   background-color: purple;" // Цвет фона навигационной панели
                "}"
                "QCalendarWidget QToolButton {"
                "   margin: 10px;" // Отступы вокруг кнопок
                "   color: white;" // Цвет текста
                "   border: none;" // Убираем границы
                "   padding: 5px;" // Отступы внутри кнопок
                "   font-size: 24pt;"
                "}"
                "QToolButton#qt_calendar_prevmonth {"
                "    qproperty-icon: url(C:/Users/User/Documents/Sprint/prev.png);"
                "}"
               " QToolButton#qt_calendar_nextmonth {"
                "    qproperty-icon: url(C:/Users/User/Documents/Sprint/next.png);"
                "}"
                );
        layout->addWidget(calendar);

        // Создаем метку для отображения выбранной даты
        QLabel *dateLabel = new QLabel("Выберите дату", this);
        calendar->setDateLabel(dateLabel); // Передаем метку в календарь
        layout->addWidget(dateLabel); // Добавляем метку в layout

        setLayout(layout);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CalendarApp window;
    window.setWindowTitle("Календарь с задачами");
    window.resize(800, 300);
    window.show();

    return app.exec();
}

#include "main.moc"
