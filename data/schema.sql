CREATE TABLE card(
    card_id INTEGER PRIMARY KEY,
    name TEXT,
    cardType TEXT,
    attribute TEXT,
    monsterType TEXT,
    monsterAbility TEXT,
    type TEXT,
    level INTEGER,
    attack INTEGER,
    defense INTEGER,
    lpendulum INTEGER,
    rpendulum INTEGER,
    spellType TEXT,
    trapType TEXT
);

CREATE TABLE deck_set(
    deck_set_id INTEGER PRIMARY KEY,
    name TEXT,
    format TEXT,
    format_date TEXT,
    primary_deck_id INTEGER,
    side_deck_id INTEGER,
    extra_deck_id INTEGER
);

CREATE TABLE deck(
    deck_id INTEGER PRIMARY KEY
);

CREATE TABLE formats (
    format_id INTEGER PRIMARY KEY,
    name TEXT,
    card_id INTEGER,
    card_status INTEGER
);

CREATE TABLE deck_to_cards(
    relation_id INTEGER PRIMARY KEY,
    deck_id INTEGER,
    card_id INTEGER
);

