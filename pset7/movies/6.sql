--determine the average rating of all movies released in 2012
SELECT avg(rating) as Average_Rating_2012 FROM ratings
JOIN movies ON ratings.movie_id = movies.id
WHERE
movies.year = 2012;