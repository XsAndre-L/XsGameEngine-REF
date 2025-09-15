#pragma once

#include <cstdint>

// ─────────────────────────────────────────────────────────────
// rgb - Represents a basic color using Red, Green, and Blue
//
// Description:    Each channel ranges from 0 to 255 (8-bit precision)
// Usage:          Used for color representation in visuals and styling
// ─────────────────────────────────────────────────────────────
struct rgb {
    uint8_t r, g, b;

    rgb(uint8_t r, uint8_t g, uint8_t b)
        : r(r)
        , g(g)
        , b(b)
    {
    }
};

// ─────────────────────────────────────────────────────────────
// rgba - Extends rgb with Alpha (transparency)
//
// Description:    Adds alpha for transparency control; same 0–255 range
// Usage:          Use when you need transparency blending or layer effects
// Behavior:       Includes implicit conversion from rgb with optional alpha
// Defaults:       Alpha defaults to 255 (fully opaque)
// ─────────────────────────────────────────────────────────────
struct rgba {
    uint8_t r, g, b, a;

    rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {
    }

    rgba(const rgb& color, uint8_t a = 255)
        : r(color.r)
        , g(color.g)
        , b(color.b)
        , a(a)
    {
    }
};

// ─────────────────────────────────────────────────────────────
// Base Terminal Colors - Standard UI/CLI visual colors
//
// Description:    Common foreground colors for text output
// Usage:          Use for basic highlighting or theming
// ─────────────────────────────────────────────────────────────

const rgb RED(205, 0, 0);
const rgb GREEN(0, 205, 0);
const rgb BLUE(0, 80, 255);
const rgb YELLOW(205, 205, 0);
const rgb ORANGE(255, 166, 64);
const rgb PURPLE(128, 0, 128);
const rgb MAGENTA(205, 0, 205);
const rgb MAGENTA_MUTED(160, 40, 160);
const rgb CYAN(0, 205, 205);
const rgb TEAL(0, 128, 128);
const rgb BROWN(139, 69, 19);
const rgb NAVY(0, 0, 128);
const rgb GRAY(127, 127, 127);
const rgb BLACK(0, 0, 0);
const rgb WHITE(255, 255, 255);
const rgb AMETHYST(99, 60, 140);
const rgb ORCHID(130, 50, 150);

// ─────────────────────────────────────────────────────────────
// Light Variants - Softened versions of base colors
//
// Description:    Lighter tints for backgrounds, low-contrast text,
//                 and light themes.
// Usage:          Ideal for subtle accents or hover states.
// ─────────────────────────────────────────────────────────────
const rgb RED_LIGHT(255, 102, 102);
const rgb GREEN_LIGHT(144, 238, 144);
const rgb BLUE_LIGHT(135, 206, 250);
const rgb YELLOW_LIGHT(255, 255, 153);
const rgb PURPLE_LIGHT(216, 191, 216);
const rgb MAGENTA_LIGHT(255, 153, 255);
const rgb CYAN_LIGHT(224, 255, 255);
const rgb GRAY_LIGHT(200, 200, 200);
const rgb BLACK_LIGHT(10, 10, 10); // soft black

// ─────────────────────────────────────────────────────────────
// Dark Variants - Deeper versions of base colors
//
// Description:    Darkened tones for backgrounds, shadows,
//                 and dark theme compatibility.
// Usage:          Suitable for text on light backgrounds or strong contrast.
// ─────────────────────────────────────────────────────────────
const rgb RED_DARK(139, 0, 0);
const rgb GREEN_DARK(0, 130, 0);
const rgb BLUE_DARK(0, 0, 139);
const rgb YELLOW_DARK(153, 153, 0);
const rgb PURPLE_DARK(75, 0, 130);
const rgb MAGENTA_DARK(139, 0, 139);
const rgb CYAN_DARK(0, 139, 139);
const rgb GRAY_DARK(100, 100, 100);
// const rgb GRAY_DARK(50, 50, 50);
const rgb WHITE_DARK(200, 200, 200); // muted white
const rgb MIDNIGHT_PURPLE(40, 0, 55);
const rgb AWKWARD_PURPLE(80, 0, 110);
const rgb ORCHID_DARK(114, 44, 132);

// ─────────────────────────────────────────────────────────────
// Accent Colors - Specialized hues for thematic use
//
// Description:    Rich, expressive tones used to highlight or brand.
// Usage:          Apply in tags, labels, themes, or non-core status elements.
// ─────────────────────────────────────────────────────────────
const rgb GOLD(255, 215, 0); // Royal or award highlight
const rgb LIME(191, 255, 0); // Vibrant and energetic lime green
const rgb INDIGO(75, 0, 130); // Deep spiritual purple-blue
const rgb PINK(255, 105, 180); // Friendly, expressive tone
const rgb VIOLET(143, 0, 255);
const rgb OLIVE(128, 128, 0);

// ─────────────────────────────────────────────────────────────
// Semantic Aliases - Abstract role-based identifiers
//
// Description:    Use these as high-level styling tokens to allow
//                 for future theme changes or overrides.
// Usage:          Prefer semantic names in UI logic for flexibility.
// ─────────────────────────────────────────────────────────────
const rgb COLOR_SUCCESS = GREEN;
const rgb COLOR_WARNING = YELLOW;
const rgb COLOR_ERROR = RED;
const rgb COLOR_INFO = CYAN;

constexpr const char8_t* BLOCK = u8"\u2588";