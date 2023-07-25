#ifndef GORA_PARSER

/// Indicates the valid types for tokens
enum TokenType {
    IDENTIFIER,
    LITERAL,
};

/// Represents a correctly parsed token
struct token {
    enum TokenType type;
    char           *value;
};

struct token* gora_parse(char *stream);

#endif // GORA_PARSER
