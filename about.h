#ifndef ABOUT_H
#define ABOUT_H
#include <QDialog>

namespace Ui {
class about;
}

class about : public QDialog
{
    Q_OBJECT

public:
    explicit about(QWidget *parent = nullptr);
    ~about();

private:
    Ui::about *ui;
};

#endif // ABOUT_H
