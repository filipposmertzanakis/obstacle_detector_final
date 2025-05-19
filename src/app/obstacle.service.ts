import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { HttpClient } from '@angular/common/http';
import { map, tap } from 'rxjs/operators';

interface DistanceResponse {
  feeds: { field1: string }[]; 
}

@Injectable({
  providedIn: 'root'
})
export class ObstacleService {
  private readonly apiUrl = 'https://api.thingspeak.com/channels/2955606/fields/1.json?api_key=WMG4LZ2CEPJ8X8TW&results=2';

  constructor(private httpClient: HttpClient) {}

  // παιρνω τα δεδομενα απο το thingspeak μεσω του write api key
  getDistance(): Observable<number> {
    return this.httpClient.get<DistanceResponse>(this.apiUrl).pipe(
      map(response => {
        // αποθηκευω την τελευταια μετρηση
        const lastFeed = response.feeds[response.feeds.length - 1];
        return lastFeed ? parseFloat(lastFeed.field1) : 0; // εαν δεν υπαρχει γυρναω 0 
      })
    );
  }
}
