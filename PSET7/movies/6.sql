SELECT AVG(rating) from ratings where movie_id IN (SELECT id from movies WHERE year = 2012)