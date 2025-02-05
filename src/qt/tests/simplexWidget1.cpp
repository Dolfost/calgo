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
				<< w->function() << "\t | " << (w->maximize() ? "max" : "min") << std::endl;
			w->variables().showSystem(w->constraints()) << std::endl;
		}
	);

	w->setFunction(ca::vec<double>{-2, 3, 0, 0, 0});
	w->setConstraints(ca::vec<double>{2, 10, 4});
	w->setVariables(ca::mat<double>{
		{-1, 2, -1, 0, 0},
		{-5, 2, 0, 1, 0},
		{1, 2, 0, 0, -1},
	});

	ca::mat<double> v = w->variables();
	ca::vec<double> c = w->constraints();
	ca::vec<double> f = w->function();
	std::cout << "f: " << f
		<< '\n'; 
	v.showSystem(c);


	wnd.setCentralWidget(w);
	wnd.show();

	return app.exec();
}
