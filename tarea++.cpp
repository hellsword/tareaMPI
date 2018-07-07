// Un simple ejemplo de uso de la plantilla de clase list
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <list>

using namespace std;

int main(int argc, char *argv[])
{
    list<double> lalista;
    double num, suma=0;

    cout << "Una sencilla calculadora" << endl;

    do
    {
      cout << "Ingrese un número, 0 para salir: ";
      cin  >> num;
      if (num != 0) lalista.push_back(num);
    }
    while (num != 0);

    cout << "----------" << endl;

    while( !lalista.empty() )
    {
      num = lalista.front();
      cout << setw(10) << num << endl;
      suma += num;
      lalista.pop_front();
    }
    cout << "----------" << endl;

    cout << setw(10) << suma << endl;

    system("PAUSE");
    return EXIT_SUCCESS;
}