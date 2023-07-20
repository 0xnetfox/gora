#ifndef GORA_PARSER

/// Indicates the valid types for tokens
enum TokenType {
    IDENTIFIER,
};

/// Represents a correctly parsed token
struct token {
    enum TokenType type;
    char           *value;
};

#endif // GORA_PARSER
