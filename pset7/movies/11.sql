--list the titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated.
SELECT title FROM ratings
JOIN movies ON movies.id = ratings.movie_id
WHERE
ratings.movie_id IN
(SELECT movies.id FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON movies.id = stars.movie_id
WHERE
people.name = 'Chadwick Boseman')
ORDER BY rating DESC
LIMIT 5;