#include <stdio.h>
#include <assert.h>

#include "../src/dfa.h"
#include "../src/nfa.h"

int main(){

    // Basic matching
    assert(nfaMatch("a", "a") > 0);
    assert(nfaMatch("b", "a") == 0);
    assert(dfaMatch("a", "a") == nfaMatch("a", "a"));

    // Matching any sequence
    assert(nfaMatch("aaaa", "a*") > 0);
    assert(dfaMatch("aaaa", "a*") == nfaMatch("aaaa", "a*"));

    // Matching with alternation
    assert(nfaMatch("a", "a|b") > 0);
    assert(nfaMatch("b", "a|b") > 0);
    assert(nfaMatch("c", "a|b") == 0);

    // Empty string
    // assert(nfaMatch("", "a*") == true);
     /*assert(nfaMatch("", "a") == false);*/
     /*assert(dfaMatch("", "a*") == nfaMatch("", "a*"));*/
     /*assert(dfaMatch("", "a") == nfaMatch("", "a"));*/

    // Start and end matching
    assert(nfaMatch("abc", "a(b|c)*") > 0);
    assert(dfaMatch("abc", "a(b|c)*") == nfaMatch("abc", "a(b|c)*"));

    // Pattern ending with a fixed string
    assert(nfaMatch("bbaaabb", "(a|b)*abb") > 0);
    assert(nfaMatch("bbaaab", "(a|b)*abb") == 0);

    // Repeated groups
    assert(nfaMatch("abab", "(ab)*") > 0);
    assert(nfaMatch("aba", "(ab)*") == 0);
    assert(dfaMatch("abab", "(ab)*") == nfaMatch("abab", "(ab)*"));

    // Patterns with Kleene star and concatenation
    assert(nfaMatch("aaaabbbb", "a*b*") > 0);
    assert(nfaMatch("aaaabbbb", "a*b*c") == 0);

    // Multiple alternations
    assert(nfaMatch("d", "a|b|c|d") > 0);
    assert(nfaMatch("e", "a|b|c|d") == 0);

    // Nested alternations
    assert(nfaMatch("ab", "(a|b)(a|b)") > 0);
    assert(nfaMatch("ac", "(a|b)(a|b)") == 0);

    // Empty input and patterns
    // assert(nfaMatch("", "") == true);
    // assert(dfaMatch("", "") == nfaMatch("", ""));
    // assert(nfaMatch("a", "") == false);

    // Patterns with redundant operators
    assert(nfaMatch("a", "a|a") > 0);
    assert(nfaMatch("aaa", "a*|a*") > 0);
    assert(dfaMatch("a", "a|a") == nfaMatch("a", "a|a"));

    // Single character inputs
    assert(nfaMatch("a", "a*") > 0);
    assert(nfaMatch("b", "a*") == 0);

    // Long matching strings
    assert(nfaMatch("aaaaaaaaaa", "a*") > 0);
    assert(dfaMatch("aaaaaaaaaa", "a*") == nfaMatch("aaaaaaaaaa", "a*"));

    // Long non-matching strings
    assert(nfaMatch("bbbbbbbbbb", "a*") == 0);
    assert(dfaMatch("bbbbbbbbbb", "a*") == nfaMatch("bbbbbbbbbb", "a*"));

    // Large input with alternation
    assert(nfaMatch("abababababababababab", "(a|b)*") > 0);
    assert(dfaMatch("abababababababababab", "(a|b)*") == nfaMatch("abababababababababab", "(a|b)*"));

    // Escaped characters
    // assert(nfaMatch("a|b", "a\\|b") == true);  // `|` as a literal
    // assert(nfaMatch("ab", "a\\*b") == false);  // `*` as a literal

    // Overlapping patterns
    assert(nfaMatch("abb", "(ab)*b") > 0);
    assert(dfaMatch("abb", "(ab)*b") == nfaMatch("abb", "(ab)*b"));

    // Different starting states
    assert(nfaMatch("b", "(a|b)*") > 0);
    assert(dfaMatch("b", "(a|b)*") == nfaMatch("b", "(a|b)*"));

    printf("regex test passed\n");
    return 0;
}
