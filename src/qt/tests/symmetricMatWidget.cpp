#include <calgo/qt/matWidget.hpp> 
#include <calgo/qt/symmetricMatModel.hpp> 

#include <calgo/mat.hpp> 

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QSpinBox>
#include <QLabel>

ca::mat<double> mat = {
	{5, 7, 11},
	{8, 3, 15},
	{12, 9, 2}
};

static std::size_t c = 0;

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	QMainWindow w;
	QWidget* wid = new QWidget;
	QVBoxLayout* lay = new QVBoxLayout;
	wid->setLayout(lay);

	auto rows = new QSpinBox;
	rows->setValue(3);
	rows->setSingleStep(1);
	auto cols = new QSpinBox;
	cols->setValue(3);

	auto spinlay = new QHBoxLayout;
	spinlay->addWidget(new QLabel("rows"));
	spinlay->addWidget(rows);
	spinlay->addWidget(new QLabel("cols"));
	spinlay->addWidget(cols);


	auto mtrx = new ca::qt::MatWidget();
	mtrx->setModel(new ca::qt::SymmetricMatModel(mat));

	QObject::connect(
		rows, &QSpinBox::valueChanged, 
		[mtrx](int val) {
			mtrx->setRowCount(val);
		}
	);
	QObject::connect(
		cols, &QSpinBox::valueChanged, 
		[mtrx](int val) {
			mtrx->setColumnCount(val);
		}
	);

	QObject::connect(
		mtrx->model(), &ca::qt::MatModel::dataChanged,
		[mtrx]() {
			std::cout <<"edit No: " <<  ++c  <<  " | mat " 
				<< mtrx->matrix().rows() << 'x' << mtrx->matrix().cols() 
				<< '\n' << mtrx->matrix() << std::endl;
		}
	);

	lay->addWidget(mtrx);
	lay->addLayout(spinlay);

	w.setCentralWidget(wid);

	w.show();

	return app.exec();
}
