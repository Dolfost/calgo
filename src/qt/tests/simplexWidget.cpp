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

	ca::mat<double> m = {
		{1, 2, 3, 4, 5, 6},
		{7, 8, 9, 10, 11, 12},
		{13, 14, 15, 16, 17, 18},
		{19, 20, 21, 22, 23, 24},
	};

	ca::vec<double> h = {101, 102, 103, 104};
	ca::vec<double> f = {-1, -2, -3, -4, -5, -6};

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
