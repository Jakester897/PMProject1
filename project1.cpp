#include<iostream>
using namespace std;

void display(double* a) //Displays array from the pointer
{
  for (int i = 0; i < *(a+1); i++)
  {
    for (int j = 0; j < *a; j++)
    {
      cout << *(a + (i * (int)(*a)) + j + 2) << "  ";
    }
    cout << endl;
  }
}

double getVal(double* a, int i, int j) //Returns value at (i,j)
{
  return *(a + 2 + (j * (int)*(a)) + i);
}

int getOff(double* a, int i, int j) //Returns offset needed to get to (i,j)
{
  return 2 + (j * (int)*(a)) + i;
}

int getSize(double* a) //Returns total number of entries in matrix, plus 2 for the dimensions
{
  return *a * *(a+1) + 2;
}

double * matrix(int num) //Initializes matrices
{
  int rows, columns;

  if (num > 0) //For dynamic labelling
    cout << "Matrix " << num << ": " << endl;
  else
    cout << "Entering matrix: " << endl;

  cout << "How many columns? " << endl;
  cin >> columns;
  cout << "How many rows? " << endl;
  cin >> rows;

  double * a = (double*)calloc(rows*columns+2, sizeof(double)); //Encodes dimensions and entries in one pointer
  *(a)=columns;
  *(a+1)=rows; //Sets first two entries to dimensions


  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++)
    {
      cout << "Element (" << j << ", " << i << "): ";
      cin >> *(a + 2 + i * columns + j); //Asks for entries in row-major order
    }

  display(a);

return a;
}

void add(double* a, double* b, bool isSum)
{
  if (*a == *b && *(a+1) == *(b+1)) //Checks if dimensions are the same
  {
    if (isSum)
      cout << "Sum: " << endl;
    else //Function will be used within subtraction function
      cout << "Difference: " << endl;
    double * sum = (double*)calloc(*a * *(a+1) + 2, sizeof(double));
    *sum = *b;
    *(sum + 1) = *(b+1);

    for (int i = 2; i < *a * *(a+1) + 2; i++)
        *(sum + i) = *(a + i) + *(b + i);

    display(sum);
  }
  else
    cout << "Invalid dimensions. Matrices must be of same size." << endl;
}

void subtract(double* a, double* b)
{
  double * temp = (double*)calloc(*b * *(b+1) + 2, sizeof(double));
  *temp = *b;
  *(temp + 1) = *(b + 1); //Dimensions of temp are the same as b
  for (int i = 2; i < 2 + (int)(*b) * (int)(*(b+1)); i++)
    *(temp + i) = *(b+i) * -1; //Values of b are all negated and placed into temp

  add(a, temp, 0); //Adds temp to a
}

double * transpose(double* a)
{
  double * temp = (double*)calloc(*a * *(a+1) + 2, sizeof(double));
  *temp = *(a+1);
  *(temp + 1) = *a; //Dimensions are flipped

  for (int i = 0; i < *(temp); i++)
    for (int j = 0; j < *(temp + 1); j++)
      *(temp + getOff(temp, i, j)) = getVal(a, j, i); //Sets necessary values

  return temp;
}

void multiply(double* a, double* b)
{
  if (*a == *(b+1)) //Checks if product exists based on the dimensions
  {
    double * product = (double*)calloc(*(a+1) * *b + 2, sizeof(double));
    *product = *b;
    *(product + 1) = *(a+1); //Sets proper dimensions for product matrix

    for (int i = 0; i < *product; i++)
      for (int j = 0; j < *(product + 1); j++)
      {
        double sum = 0;
        for(int m = 0; m < *a; m++)
          sum += getVal(a, m, j) * getVal(b, i, m); //Multiplies row by column and adds them
        *(product + getOff(product, i, j)) = sum;
      }
    cout << "Product: " << endl;
    display(product);
  }
  else
    cout << "Invalid dimensions. First matrix must have as many columns as second matrix has rows." << endl;
}

double * submatrix (double* a, int i, int j)
{
if (*a-1 > 0 && *(a+1)-1 > 0) //Ensures a submatrix can exist
{
  double * temp = (double*)calloc((*a - 1)*(*(a+1)-1) + 2, sizeof(double));
  *temp = *a - 1;
  *(temp+1) = *(a+1) - 1; //Both dimensions one less than input

  int k = 2; //Starts at offset past dimension entries
  while (k < getSize(temp))
    for (int m = 0; m < *(a+1); m++)
      for (int n = 0; n < *a; n++)
        if (m != j && n != i)
        {
          *(temp+k) = getVal(a, n, m); //Fills temp only if entry is not in the blocked off row/column
          k++;
        }

  return temp;
}
else
{
  cout << "No submatrix exists." << endl;
  return a;
}
}
double determinant (double * a)
{
  if (*a != *(a+1)) //Matrix needs to be square to avoid this
  {
    cout << "Determinant does not exist." << endl;
    return 0; //Returns placeholder of 0 in case value is used later
  }

  if (*a == 1)
    return getVal(a, 0, 0); //Base case: determinant is the value

  double deter = 0;
  int k = 1;
  for (int i = 0; i < *a; i++) //Uses recursion to do cofactor expansion
  {
    deter += k * getVal(a,i,0) * determinant(submatrix(a, i, 0));
    k *= -1; //Ensures proper signs
  }

  return deter;
}

double * inverse (double * a)
{
  if (*a != *(a+1))
  {
    cout << "Inverse does not exist. Matrix is not square." << endl;
    return a; //Returns original matrix as placeholder
  }
  if (determinant(a) == 0)
  {
    cout << "Inverse does not exist. Matrix has a determinant of 0." << endl;
    return a; //Ditto
  }

  double * inv = (double*)calloc(*a * *(a+1) + 2, sizeof(double));
  *inv = *a;
  *(inv+1) = *a; //Inverse has same dimensions

  if (*a == 1)
  {
    *(inv + getOff(inv,0,0)) = 1/(*(a + getOff(inv,0,0))); //For 1x1, inverse is just reciprocal
  }
  else
  {
  for (int i = 0; i < *inv; i++)
    for (int j = 0; j < *(inv+1); j++)
    {
      if ((i + j) % 2 == 0)
        *(inv + getOff(inv, i, j)) = determinant(submatrix(a,j,i))/determinant(a);
      else
        *(inv + getOff(inv, i, j)) = -1 * determinant(submatrix(a,j,i))/determinant(a);
    }
  }
  return inv;
}
int main()
{
  cout << "Welcome to Jake Seary's Matrix Calculator!" << endl;
  int choice = 7;
    while (choice != 0) //Choosing 0 exits the loop
    {
      cout << "Main Menu" << endl;
      cout << "Choice 1: Addition" << endl;
      cout << "Choice 2: Subtraction" << endl;
      cout << "Choice 3: Multiplication" << endl;
      cout << "Choice 4: Transpose" << endl;
      cout << "Choice 5: Determinant" << endl;
      cout << "Choice 6: Inverse" << endl;
      cout << "Enter your desired operation, or type \"0\" to exit. ";
      cin >> choice;

      switch (choice)
      {
        case 1:
        {
          double * mat1 = matrix(1);
          double * mat2 = matrix(2);
          add(mat1, mat2, 1);
          delete mat1;
          delete mat2;
          break;
        }
        case 2:
        {
          double * mat1 = matrix(1);
          double * mat2 = matrix(2);
          subtract(mat1, mat2);
          delete mat1;
          delete mat2;
          break;
        }
        case 3:
        {
          double * mat1 = matrix(1);
          double * mat2 = matrix(2);
          multiply(mat1, mat2);
          delete mat1;
          delete mat2;
          break;
        }
        case 4:
        {
          double * mat = matrix(0);
          cout << "Transpose: " << endl;
          display(transpose(mat));
          delete mat;
          break;
        }
        case 5:
        {
          double * mat = matrix(0);
          cout << "Determinant: \n" << determinant(mat) << endl;
          delete mat;
          break;
        }
        case 6:
        {
          double * mat = matrix(0);
          cout << "Inverse: " << endl;
          display(inverse(mat));
          delete mat;
          break;
        }
        case 0:
        {
          cout << "Goodbye!" << endl;
          break;
        }
        default:
        {
          cout << "Invalid operation. Try again." << endl;
          break;
        }
      }
  }
    return 0;
}
