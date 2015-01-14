package com.example.ygodeck;

import java.util.ArrayList;

public interface DeckContent {
    public void addCards(ArrayList<String> cards);
    public void addCard(String card);
    public void removeCard(String card);
}
