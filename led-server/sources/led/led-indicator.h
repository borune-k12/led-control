#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H

#include <QWidget>
#include <QTimer>
#include "led-base.h"

using namespace Led;

class LedIndicator : public QWidget, public LedBase {

  Q_OBJECT

 private:
  QColor color_qt_;
  QTimer timer_;
  bool current_state_;

 public:
  explicit LedIndicator(QWidget* parent = nullptr);

  void set_state(const State state) override;
  State get_state() const override;

  void set_color(const Color color) override;
  Color get_color() const override;

  void set_rate(const Rate rate) override;
  Rate get_rate() const override;

  void set_error(const bool error) override;

 protected:
  void paintEvent(QPaintEvent*) override;

 signals:
  void rate_changed(const Rate rate);

 private slots:
  void change_rate(const Rate rate) noexcept;
};

#endif // LED_INDICATOR_H
