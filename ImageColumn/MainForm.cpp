#include "MainForm.h"

[ImageColumn::STAThreadAttribute]
int main()
{
	ImageColumn::MainForm ^fm = gcnew ImageColumn::MainForm();
	fm->ShowDialog();
	return 0;
}
