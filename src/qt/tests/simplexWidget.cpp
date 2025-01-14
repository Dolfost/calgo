#include <calgo/qt/matWidget.hpp> 

#include <calgo/qt/simplexWidget.hpp> 

#include <iostream> 

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QSpinBox>
#include <QLabel>

static std::size_t c = 0;

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	QMainWindow wnd;

	auto w = new ca::qt::SimplexWidget;
	QObject::connect(
		w, &ca::qt::SimplexWidget::systemChanged,
		[w]() {
			std::cout << "change No: " << ++c << '\n'
				<< w->function() << "\t | " << (w->max() ? "max" : "min") << std::endl;
			w->variables().showSystem(w->constraints()) << std::endl;
		}
	);
	wnd.setCentralWidget(w);
	wnd.show();

	return app.exec();
}
