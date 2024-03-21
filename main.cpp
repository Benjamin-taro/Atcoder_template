<%!
    import json
    import os
    import platform
    import shutil
    from logging import getLogger

    import onlinejudge_template.generator.cplusplus as cplusplus
    import onlinejudge_template.generator.topcoder as topcoder
    import onlinejudge_template.generator.about as about
    import onlinejudge_template.generator.hook as hook
%>\
<%
    logger = getLogger(__name__)
    data["config"]["rep_macro"] = "REP"
    data["config"]["long_long_int"] = "int64_t"
    if platform.system() == "Linux" and "clang" not in os.environ.get("CXX", "g++"):
        include = "#include <bits/stdc++.h>"
    else:
        include = "\n".join([
            "#include <iostream>",
            "#include <string>",
            "#include <vector>",
            "#include <algorithm>",
            "#include <iostream>",
            "#include <vector>",
            "#include <string>",
            "#include <utility>",
            "#include <queue>",
            "#include <array>",
            "#include <climits>",
            "#include <cmath>",
            "#include <set>",
            "#include <map>",
            "#include <bitset>",
            "#include <deque>",
            "#include <numeric>",
            "#include <assert.h>",
            "#include <stack>",
            "#include <unordered_map>",
            "#include <type_traits> // For std::is_floating_point",
            "#include <cmath> // For std::ceil",
            "#include <cstring>" ,
        ])
    if not shutil.which("clang-format"):
        logger.warning("clang-format is not installed. If you want to generate well-formatted code, please install it. If you use Ubuntu, you can run $ sudo apt install clang-format")
    else:
        format_config = {
            "BasedOnStyle": "Google",
            "IndentWidth": 4,
            "ColumnLimit": 9999,
            "ForEachMacros": ["REP", "REP3", "REP_R", "REP3R"],
        }
        hook.register_filter_command(["clang-format", "--style", json.dumps(format_config)], data=data)
%>\
${include}
#define REP(i, n) for (int i = 0; (i) < (int)(n); ++ (i))
#define REP3(i, m, n) for (int i = (m); (i) < (int)(n); ++ (i))
#define REP_R(i, n) for (int i = (int)(n) - 1; (i) >= 0; -- (i))
#define REP3R(i, m, n) for (int i = (int)(n) - 1; (i) >= (int)(m); -- (i))
#define ALL(x) ::std::begin(x), ::std::end(x)
using namespace std;
//文字配列の二次元配列みたいなやつを回転させる。
vector<string> rotate90(const vector<string>& matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    vector<string> rotated(m, string(n, '.'));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            rotated[j][n - 1 - i] = matrix[i][j];
        }
    }
    return rotated;
}

// Data structures and algorithms for disjoint set union problems
struct dsu {
  public:
    dsu() : _n(0) {}
    explicit dsu(int n) : _n(n), parent_or_size(n, -1) {}

    int merge(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        int x = leader(a), y = leader(b);
        if (x == y) return x;
        if (-parent_or_size[x] < -parent_or_size[y]) std::swap(x, y);
        parent_or_size[x] += parent_or_size[y];
        parent_or_size[y] = x;
        return x;
    }

    bool same(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        return leader(a) == leader(b);
    }

    int leader(int a) {
        assert(0 <= a && a < _n);
        if (parent_or_size[a] < 0) return a;
        return parent_or_size[a] = leader(parent_or_size[a]);
    }

    int size(int a) {
        assert(0 <= a && a < _n);
        return -parent_or_size[leader(a)];
    }

    std::vector<std::vector<int>> groups() {
        std::vector<int> leader_buf(_n), group_size(_n);
        for (int i = 0; i < _n; i++) {
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]]++;
        }
        std::vector<std::vector<int>> result(_n);
        for (int i = 0; i < _n; i++) {
            result[i].reserve(group_size[i]);
        }
        for (int i = 0; i < _n; i++) {
            result[leader_buf[i]].push_back(i);
        }
        result.erase(
            std::remove_if(result.begin(), result.end(),
                           [&](const std::vector<int>& v) { return v.empty(); }),
            result.end());
        return result;
    }

  private:
    int _n;
    // root node: -1 * component size
    // otherwise: parent
    std::vector<int> parent_or_size;
};

//listing the prime numbers until N
vector<int64_t> findPrimes(int64_t N) {
    vector<bool> isPrime(N + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int64_t i = 2; i * i <= N; ++i) {
        if (isPrime[i]) {
            for (int64_t j = i * i; j <= N; j += i) {
                isPrime[j] = false;
            }
        }
    }
    vector<int64_t> primes;
    for (int64_t i = 2; i <= N; ++i) {
        if (isPrime[i]) primes.push_back(i);
    }
    return primes;
}

//Checking given number is prime or not
bool IsPrime(int num)
{
    if (num < 2) return false;
    else if (num == 2) return true;
    else if (num % 2 == 0) return false; // 偶数はあらかじめ除く

    double sqrtNum = sqrt(num);
    for (int i = 3; i <= sqrtNum; i += 2)
    {
        if (num % i == 0)
        {
            // 素数ではない
            return false;
        }
    }

    // 素数である
    return true;
}

${cplusplus.declare_constants(data)}
% if topcoder.is_topcoder(data):
<% solve_function = topcoder.class_name(data) + "()." + topcoder.method_name(data) %>\
class ${topcoder.class_name(data)} {
public:
    ${cplusplus.return_type(data)} ${topcoder.method_name(data)}(${cplusplus.formal_arguments(data)}) {
        // TODO: edit here
    }
};
% else:
<% solve_function = "solve" %>\
${cplusplus.return_type(data)} solve(${cplusplus.formal_arguments(data)}) {
    // TODO: edit here
}
% endif

// generated by ${about.title} ${about.version} (${about.url})
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
${cplusplus.read_input(data)}
    auto ${cplusplus.return_value(data)} = ${solve_function}(${cplusplus.actual_arguments(data)});
${cplusplus.write_output(data)}
    return 0;
}
