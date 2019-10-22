#include "led-indicator.h"
#include <QPainter>

static const std::map<Color, QColor> COLORS2QtCOLORS = {
  {Color::Color_Red, Qt::red},
  {Color::Color_Green, Qt::green},
  {Color::Color_Blue, Qt::blue}
};

static const QSize LED_SIZE(75, 75);
static const int BORDER_WIDTH = 5;
static const QPointF GRADIENT_CENTER(LED_SIZE.width()*0.3, LED_SIZE.height()*0.3);
static const QColor OFF_COLOR(Qt::darkGray);
static const QColor ERROR_COLOR(128, 128, 128, 128);

LedIndicator::LedIndicator(QWidget* parent) :
  QWidget(parent),
  color_qt_(Qt::red),
  current_state_(false) {
  state_ = State::State_Off;
  color_ = Color::Color_Red;
  rate_ = 0;

  setFixedSize(LED_SIZE);
  timer_.setSingleShot(false);

  connect(&timer_, &QTimer::timeout, [this]() {
    current_state_ = !current_state_ && (state_ == State::State_On);
    update();
  });

  connect(this, &LedIndicator::rate_changed, this, &LedIndicator::change_rate, Qt::QueuedConnection);
}

void LedIndicator::set_state(const State state) {
  check_has_error();
  state_ = state;
  current_state_ = (state == State::State_On);
  update();
}

State LedIndicator::get_state() const {
  check_has_error();
  return state_;
}

void LedIndicator::set_color(const Color color) {
  check_has_error();

  const auto it = COLORS2QtCOLORS.find(color);
  if (it != COLORS2QtCOLORS.cend()) {
    color_ = color;
    color_qt_ = it->second;
  }
  update();
}

Color LedIndicator::get_color() const {
  check_has_error();  
  return color_;
}

void LedIndicator::set_rate(const Rate rate) {
  check_has_error();
  rate_ = rate;

  emit rate_changed(rate);
}

Rate LedIndicator::get_rate() const {
  check_has_error();
  return rate_;
}

void LedIndicator::set_error(const bool error) {
  LedBase::set_error(error);
  update();
}

void LedIndicator::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QPainterPath path;
  path.addEllipse(rect().adjusted(+BORDER_WIDTH, +BORDER_WIDTH, -BORDER_WIDTH, -BORDER_WIDTH));

  QRadialGradient gradient(GRADIENT_CENTER, width()/2);
  gradient.setColorAt(0, Qt::white);
  gradient.setColorAt(1, current_state_ ? color_qt_ : OFF_COLOR);

  painter.fillPath(path, gradient);

  QPen pen(QColor(), BORDER_WIDTH);
  painter.setPen(pen);
  painter.drawPath(path);

  if (has_error_) {
    painter.fillPath(path, ERROR_COLOR);
  }
}

void LedIndicator::change_rate(const Rate rate) noexcept {
  if (rate > 0) {
    timer_.setInterval(1000/rate);
    if (!timer_.isActive()) {
      timer_.start();
    }
  } else {
    timer_.stop();
    current_state_ = (state_ == State::State_On);
    update();
  }
}
