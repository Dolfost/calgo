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

	ca::Mat<double> m = {
		{1, 2, 3, 4, 5, 6},
		{3, 4, 5, 6, 7, 8},
		{1, 4, 3, 6, 5, 7},
		{4, 5, 6, 5, 9, 9},
	};

	ca::Vec<double> h = {9, 8, 9, 11};
	ca::Vec<double> f = {6, 6, 6, 6, 6, 6};

	auto w = new ca::qt::SimplexWidget;
	QObject::connect(
		w, &ca::qt::SimplexWidget::systemChanged,
		[w]() {
			std::cout << "change No: " << ++c << '\n'
				<< w->function() << "\t | " << (w->maximize() ? "max" : "min") << std::endl;
			w->variables().showSystem(w->constraints()) << std::endl;
		}
	);
	w->setVariables(m);
	w->setConstraints(h);
	w->setFunction(f);
	wnd.setCentralWidget(w);
	wnd.show();

	return app.exec();
}
