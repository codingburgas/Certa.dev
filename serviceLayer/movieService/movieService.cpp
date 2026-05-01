#include "movieService.h"
#include "movieRepository.h"

namespace MovieService {
    MovieResponse getAllGenres() {
        return MovieRepository::getAllGenres();
    }

    GetMoviesResponse getAllMovies() {
        return MovieRepository::getAllMovies();
    }
}
