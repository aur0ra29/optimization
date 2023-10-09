#include <bits/stdc++.h>
using namespace std;
const int N = 101, INF = INT_MAX;
int n, m, d, basic[N], temp, Xb, Xk, comp, min_ind, Xnon[N], enter, leave, q;
char c[100];
string s;
double A[N][N], b[N], tableau[N][N], delta[N], mn, rat, eps;
bool mx;
void invalid_input() {
    printf("invalid input\n");
    exit(0);
}
void not_applicable() {
    printf("The method is not applicable!\n");
    for(int i=0; i<=m; i++) {
        for(int j=0; j<=comp; j++) {
            printf("%lf ", tableau[i][j]);
        }
        printf("\n");
    }
    exit(0);
}
bool is_pos_integer(string s) {
    for(int i=0; i<s.size(); i++) {
        if(i==0 && s[0]=='-') continue;
        if(s[i]<'0' || s[i]>'9')
            return 0;
    }
    return 1;
}
bool is_double(string s) {
    bool dot = 0;
    for(int i=0; i<s.size(); i++) {
        if(i==0 && s[0]=='-') continue;
        if(i==1 && s[1]=='.' && s[0]=='-') return 0;
        if(s[i] == '.') {
            if(dot) return 0;
            dot = 1;
            continue;
        }
        if(s[i]<'0' || s[i]>'9') return 0;
    }
    return 1;
}
int main() {
    //enter the number of variables
    printf("enter the number of variables\n");
    scanf("%s", c);
    s = c;
    if(is_pos_integer(s)) {
        n = stoi(s);
    } else {
        invalid_input();
    }
    //enter the number of constraints
    printf("enter the number of constraints\n");
    scanf("%s", c);
    s = c;
    if(is_pos_integer(s)) {
        m = stoi(s);
    } else {
        invalid_input();
    }
    //initialize the non basic variables
    for(int i=1; i<=m; i++) Xnon[i] = i;
    d = n + m;

    A[0][0] = 1;
    for(int i=1; i<=m; i++) {
        A[i][0] = 0;
    }
    //max or min?
    printf("if it's maximization type \'max\' and if it\'s minimization type \'min\'\n");
    scanf("%s", c);
    s = c;
    if(s == "max") mx = 1;
    else if (s == "min") mx = 0;
    else invalid_input();
    //enter coefficients of the objective function
    printf("enter coefficients of the objective function\n");
    for(int j=1; j<=n; j++) {
        scanf("%s", c);
        s = c;
        if(is_double(s)) {
            A[0][j] = stod(s);
        } else {
            invalid_input();
        }
        if(mx) A[0][j] *= -1;
    }

    //enter coefficients of constraints
    for(int i=1; i<=m; i++) {
        printf("enter coefficients of the constraint number %d\n", i);
        for(int j=1; j<=n; j++) {
            scanf("%s", c);
            s = c;
            if(is_double(s)) {
                A[i][j] = stod(s);
            } else {
                invalid_input();
            }
        }
    }

    //initialize basic columns
    for(int j=n+1; j<=d; j++) {
        temp++;
        for(int i=0; i<=m; i++) {
            A[i][j] = (i == temp ? 1 : 0);
        }
    }

    b[0] = 0;
    //enter the coefficients vector b
    printf("enter coefficients of the right hand side\n");
    for(int i=1; i<=m; i++) {
        scanf("%s", c);
        s = c;
        if(is_double(s)) {
            b[i] = stod(s);
            if(b[i] < 0) {
                invalid_input();
            }
        } else {
            invalid_input();
        }
    }

    //initialize the tableau
    tableau[0][0] = 0;
    for(int i=1; i<=m; i++) {
        tableau[i][0] = n + i;
    }
    temp = 0;
    for(int j=1; j<=m+1; j++) {
        for(int i=0; i<=m; i++) {
            tableau[i][j] = (i == temp? 1 : 0);
        }
        temp++;
    }

    //enter epsilon
    printf("enter epsilon\n");
    scanf("%s", c);
    s = c;
    if(is_double(s)) {
        eps = stod(s);
        if(eps < 0) invalid_input();
    } else {
        invalid_input();
    }

    Xb = m+2;
    Xk = m+3;
    comp = m+4;

    for(int i=0; i<=m; i++) {
        tableau[i][Xb] = b[i];
    }

    while(true) {
        //calculate deltas and pick the minimum
        min_ind = -1;
        mn = INF;
        for (int k = 1; k <= m; k++) {
            delta[k] = 0;
            for (int j = 1; j <= m + 1; j++) {
                delta[k] += tableau[0][j] * A[j - 1][Xnon[k]];
            }
            if(fabs(delta[k]) <= eps)  delta[k] = 0;
            if (delta[k] < 0 && delta[k] < mn) {
                min_ind = k;
                mn = delta[k];
            }
        }

        //entering variable
        enter = min_ind;

        if(enter == -1) {
            break;
        }

        //initialize Xk
        for (int i = 0; i <= m; i++) {
            tableau[i][Xk] = 0;
        }

        //calculate Xk = B_1 * Pi
        for (int i = 0; i <= m; i++) {
            for (int j = 1; j <= m + 1; j++) {
                tableau[i][Xk] += tableau[i][j] * A[j-1][Xnon[min_ind]];
            }
        }

        //find the leaving column through comparing the ratio
        for (int i = 1; i <= m; i++) {
            if(fabs( tableau[i][Xk]) <= eps)  tableau[i][Xk] = 0;
            if (tableau[i][Xk] == 0) {
                tableau[i][comp] = 0;
                continue;
            }
            tableau[i][comp] = tableau[i][Xb] / tableau[i][Xk];
        }
        min_ind = -1;
        mn = INF;
        for (int i = 1; i <= m; i++) {
            if(fabs( tableau[i][comp]) <= eps)  tableau[i][comp] = 0;
            if (tableau[i][comp] > 0 && tableau[i][comp] < mn) {
                mn = tableau[i][comp];
                min_ind = i;
            }
        }

        //leaving variable
        leave = min_ind;

        if(leave == -1) {
            not_applicable();
        }

        temp = Xnon[enter];
        Xnon[enter] = (int)tableau[leave][0];
        tableau[leave][0] = temp;

        //fix the tableau
        for (int j = 1; j <= Xk; j++) {
            tableau[leave][j] /= tableau[leave][Xk];
            if(fabs( tableau[leave][j]) <= eps)  tableau[leave][j] = 0;
        }
        for (int i = 0; i <= m; i++) {
            if (i == leave) continue;
            rat = tableau[i][Xk] / tableau[leave][Xk];

            for (int j = 2; j <= Xk; j++) {
                tableau[i][j] -= rat * tableau[leave][j];
                if(fabs(tableau[i][j]) <= eps) tableau[i][j] = 0;
            }
        }
    }

    for(int i=1; i<=n; i++) {
        bool found = 0;
        printf("the decision variable X%d = ", i);
        for(int j=1; j<=m; j++) {
            if((int)tableau[j][0] == i) {
                printf("%lf\n", tableau[j][Xb]);
                found = 1;
                break;
            }
        }
        if(!found) printf("0\n");
    }
    printf("the optimal solution is Z = %lf\n", tableau[0][Xb] * (mx?1:-1));

}