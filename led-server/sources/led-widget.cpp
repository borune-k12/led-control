#include "led-widget.h"
#include <QCheckBox>
#include <QLayout>

LedWidget::LedWidget(QWidget *parent) :
  QDialog(parent),
  led_(std::make_shared<LedIndicator>()) {
  setWindowTitle("Led");

  QVBoxLayout* layout = new QVBoxLayout(this);
  QCheckBox* has_error = new QCheckBox(tr("error"));
  connect(has_error, &QCheckBox::toggled, [this](const bool checked) {
    led_->set_error(checked);
  });
  layout->addWidget(led_.get(), 0,Qt::AlignHCenter);
  layout->addWidget(has_error, 0, Qt::AlignHCenter);
}

std::shared_ptr<LedBase> LedWidget::get_led() const {
  return led_;
}


