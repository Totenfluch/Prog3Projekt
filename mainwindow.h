/**
 * @file mainwindow.h
 *
 */

/**
 * @file mainwindow.h
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

/**
 * @brief 
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 
     *
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);
    /**
     * @brief 
     *
     */
    ~MainWindow();

private slots:
    /**
     * @brief 
     *
     */
    void on_saveData_clicked();
    /**
     * @brief 
     *
     */
    void update();

    /**
     * @brief 
     *
     */
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui; /**< TODO: describe */
    QWidget* x; /**< TODO: describe */
};

#endif // MAINWINDOW_H
