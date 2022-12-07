#include <eclib/marith.h>
#include <assert.h>
#include "arith_utils.h"

void divisor_iterator::increment()
{
  if (!ok) return;
  // find first exponent which can be incremented, increment it and
  // then reset earlier ones to 0 and update partial products
  for (int ip=0; ip<np; ip++)
    {
      if (ee[ip]<EE[ip])
        {
          ee[ip]++;
          NN[ip]*=PP[ip];
          for (int jp=0; jp<ip; jp++)
            {
              ee[jp]=0;
              NN[jp]=NN[ip];
            }
          return;
        }
    }
  // we only get here when all ee[ip]==EE[ip] so the iteration stops
  ok = 0;
}


// test function for divisor_itersor
int test_divisor_iterator(const bigint& N)
{
  if (is_zero(N)) return 1;
  vector<bigint> divs1 = posdivs(N);
  vector<bigint> divs2;
  divisor_iterator divN(N);
  while (divN.is_ok())
    {
      divs2.push_back(divN.value());
      divN.increment();
    }
  std::sort(divs1.begin(), divs1.end());
  std::sort(divs2.begin(), divs2.end());
  if (divs1!=divs2)
    {
      cout << "divisors are "<< divs1 <<endl;
      cout << "new list is  "<< divs2 <<endl;
    }
  return divs1==divs2;
}

// Compute N from its factorization (lists of primes and exponents) --
// (name taken from gp)
bigint factorback(const vector<bigint>&PP, const vector<int>& EE)
{
  bigint N(1);
  auto pi =PP.begin();
  auto ei=EE.begin();
  while(pi!=PP.end())
    N *= pow(*pi++, *ei++);
  return N;
}

// Maximum conductor for a given list of primes
bigint MaxN(const vector<bigint>&PP)
{
  vector<int> EE;
  for(auto pi = PP.begin(); pi!=PP.end(); ++pi)
    {
      bigint p = *pi;
      EE.push_back((p==2?8:p==3?5:2));
    }
  return factorback(PP,EE);
}

// Squarefree part of N
bigint sqf(const bigint& N)
{
  vector<bigint> PP = pdivs(N);
  vector<int> EE(PP.size(), 1);
  return factorback(PP,EE);
}
