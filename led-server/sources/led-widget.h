#ifndef LED_WIDGET_H
#define LED_WIDGET_H
#include <memory>
#include <QDialog>
#include <led/led-indicator.h>

class LedWidget : public QDialog
{
  Q_OBJECT

 private:
  std::shared_ptr<LedIndicator> led_;

public:
  explicit LedWidget(QWidget *parent = nullptr);

  std::shared_ptr<LedBase> get_led() const;
};

#endif // LED_WIDGET_H
