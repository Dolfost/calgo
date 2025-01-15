#include <calgo/qt/matWidget.hpp> 

#include <calgo/qt/systemWidget.hpp> 

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

	ca::Mat<double> m = {
		{1, 2, 3, 4, 5, 6},
		{3, 4, 5, 6, 7, 8},
		{1, 4, 3, 6, 5, 7},
		{4, 5, 6, 5, 9, 9},
	};

	ca::Vec<double> h = {9, 8, 9, 11};

	auto w = new ca::qt::SystemWidget;
	w->setVariables(m);
	w->setConstraints(h);
	QObject::connect(
		w, &ca::qt::SystemWidget::systemChanged,
		[w]() {
			std::cout << "change No:" << ++c << '\n'; 
		w->variables().showSystem(w->constraints()) << std::endl;
		}
	);
	wnd.setCentralWidget(w);
	wnd.show();

	return app.exec();
}
