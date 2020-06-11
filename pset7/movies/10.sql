--list the names of all people who have directed a movie that received a rating of at least 9.0
SELECT DISTINCT name as Director_9 FROM people
JOIN directors ON directors.person_id = people.id
JOIN movies ON movies.id = directors.movie_id
join ratings ON movies.id = ratings.movie_id
WHERE
ratings.rating >= 9.0;