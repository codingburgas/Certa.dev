#include "movieService.h"
#include "movieRepository.h"

namespace MovieService {
    MovieResponse getAllGenres() {
        return MovieRepository::getAllGenres();
    }
}
