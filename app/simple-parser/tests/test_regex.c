#include <stdio.h>
#include <assert.h>

#include "../src/dfa.h"
#include "../src/nfa.h"

int main(){

    // Basic matching
    assert(nfaMatch("a", "a") == true);
    assert(nfaMatch("b", "a") == false);
    assert(dfaMatch("a", "a") == nfaMatch("a", "a"));

    // Matching any sequence
    assert(nfaMatch("aaaa", "a*") == true);
    assert(dfaMatch("aaaa", "a*") == nfaMatch("aaaa", "a*"));

    // Matching with alternation
    assert(nfaMatch("a", "a|b") == true);
    assert(nfaMatch("b", "a|b") == true);
    assert(nfaMatch("c", "a|b") == false);

    // Empty string
    // assert(nfaMatch("", "a*") == true);
    // assert(nfaMatch("", "a") == false);
    // assert(dfaMatch("", "a*") == nfaMatch("", "a*"));
    // assert(dfaMatch("", "a") == nfaMatch("", "a"));

    // Start and end matching
    assert(nfaMatch("abc", "a(b|c)*") == true);
    assert(dfaMatch("abc", "a(b|c)*") == nfaMatch("abc", "a(b|c)*"));

    // Pattern ending with a fixed string
    assert(nfaMatch("bbaaabb", "(a|b)*abb") == true);
    assert(nfaMatch("bbaaab", "(a|b)*abb") == false);

    // Repeated groups
    assert(nfaMatch("abab", "(ab)*") == true);
    assert(nfaMatch("aba", "(ab)*") == false);
    assert(dfaMatch("abab", "(ab)*") == nfaMatch("abab", "(ab)*"));

    // Patterns with Kleene star and concatenation
    assert(nfaMatch("aaaabbbb", "a*b*") == true);
    assert(nfaMatch("aaaabbbb", "a*b*c") == false);

    // Multiple alternations
    assert(nfaMatch("d", "a|b|c|d") == true);
    assert(nfaMatch("e", "a|b|c|d") == false);

    // Nested alternations
    assert(nfaMatch("ab", "(a|b)(a|b)") == true);
    assert(nfaMatch("ac", "(a|b)(a|b)") == false);

    // Empty input and patterns
    // assert(nfaMatch("", "") == true);
    // assert(dfaMatch("", "") == nfaMatch("", ""));
    // assert(nfaMatch("a", "") == false);

    // Patterns with redundant operators
    assert(nfaMatch("a", "a|a") == true);
    assert(nfaMatch("aaa", "a*|a*") == true);
    assert(dfaMatch("a", "a|a") == nfaMatch("a", "a|a"));

    // Single character inputs
    assert(nfaMatch("a", "a*") == true);
    assert(nfaMatch("b", "a*") == false);

    // Long matching strings
    assert(nfaMatch("aaaaaaaaaa", "a*") == true);
    assert(dfaMatch("aaaaaaaaaa", "a*") == nfaMatch("aaaaaaaaaa", "a*"));

    // Long non-matching strings
    assert(nfaMatch("bbbbbbbbbb", "a*") == false);
    assert(dfaMatch("bbbbbbbbbb", "a*") == nfaMatch("bbbbbbbbbb", "a*"));

    // Large input with alternation
    assert(nfaMatch("abababababababababab", "(a|b)*") == true);
    assert(dfaMatch("abababababababababab", "(a|b)*") == nfaMatch("abababababababababab", "(a|b)*"));

    // Escaped characters
    // assert(nfaMatch("a|b", "a\\|b") == true);  // `|` as a literal
    // assert(nfaMatch("ab", "a\\*b") == false);  // `*` as a literal

    // Overlapping patterns
    assert(nfaMatch("abb", "(ab)*b") == true);
    assert(dfaMatch("abb", "(ab)*b") == nfaMatch("abb", "(ab)*b"));

    // Different starting states
    assert(nfaMatch("b", "(a|b)*") == true);
    assert(dfaMatch("b", "(a|b)*") == nfaMatch("b", "(a|b)*"));

    printf("regex test passed\n");
    return 0;
}