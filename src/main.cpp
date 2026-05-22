#include "ui.hpp"

int main() {
  Ui *pUi = Ui::Instance();
  
  pUi->Scheduling();

  delete pUi;

  return 0;
}