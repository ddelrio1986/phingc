#ifndef PHINGC_OUTPUT_STYLES_H
#define PHINGC_OUTPUT_STYLES_H

typedef struct {
    const char *bold;
    const char *cyan;
    const char *cyan_bold;
    const char *initial;
    const char *purple;
    const char *purple_bold;
    const char *red;
    const char *red_bold;
} OutputStyle;

extern const OutputStyle output_styles;

#endif //PHINGC_OUTPUT_STYLES_H
