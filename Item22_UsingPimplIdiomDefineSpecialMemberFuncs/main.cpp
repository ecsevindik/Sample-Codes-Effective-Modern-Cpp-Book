
/*
• The Pimpl Idiom decreases build times by reducing compilation dependencies
between class clients and class implementations.
• For std::unique_ptr pImpl pointers, declare special member functions in
the class header, but implement them in the implementation file. Do this even
if the default function implementations are acceptable.
• The above advice applies to std::unique_ptr, but not to std::shared_ptr.
 */

#include "widget.h"

int main() {

    Widget w;

    Widget w2(std::move(w));

    Widget w3(w2);
    
    Widget w4;
    w4 = w3;

    return 0;
}
