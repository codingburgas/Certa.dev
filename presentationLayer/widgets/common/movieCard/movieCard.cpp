#include "movieCard.h"
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPixmapCache>
#include <QVBoxLayout>

MovieCard::MovieCard(const MovieDto &movie, QWidget *parent) : QFrame(parent), _movieId(movie.id) {
    setObjectName("movieCard");
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);

    QVBoxLayout *cardLayout = new QVBoxLayout(this);
    cardLayout->setContentsMargins(8, 8, 8, 0);
    cardLayout->setSpacing(0);

    QLabel *posterLabel = new QLabel(this);
    posterLabel->setFixedHeight(POSTER_HEIGHT);
    posterLabel->setAlignment(Qt::AlignCenter);
    posterLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    posterLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    const qreal dpr = devicePixelRatioF();
    const QString posterCacheKey = QStringLiteral("poster:%1@%2x%3")
            .arg(movie.posterPath)
            .arg(POSTER_HEIGHT)
            .arg(dpr);
    QPixmap pixmap;
    if (!QPixmapCache::find(posterCacheKey, &pixmap)) {
        QPixmap raw(movie.posterPath);
        if (!raw.isNull()) {
            pixmap = raw.scaledToHeight(static_cast<int>(POSTER_HEIGHT * dpr), Qt::SmoothTransformation);
            pixmap.setDevicePixelRatio(dpr);
            QPixmapCache::insert(posterCacheKey, pixmap);
        }
    }
    if (!pixmap.isNull()) {
        posterLabel->setPixmap(pixmap);
    }

    QWidget *infoWidget = new QWidget(this);
    infoWidget->setStyleSheet("background-color: transparent;");
    infoWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(10, 8, 10, 10);
    infoLayout->setSpacing(6);

    QLabel *titleLabel = new QLabel(movie.title, infoWidget);
    titleLabel->setObjectName("movieCardTitle");
    titleLabel->setWordWrap(true);

    QLabel *genresLabel = new QLabel(movie.genres, infoWidget);
    genresLabel->setObjectName("movieCardGenres");
    genresLabel->setWordWrap(true);

    QWidget *statsRow = new QWidget(infoWidget);
    statsRow->setStyleSheet("background-color: transparent;");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsRow);
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(4);

    QLabel *starIcon = new QLabel(statsRow);
    const QString starCacheKey = QStringLiteral("icon:star@%1x%2").arg(ICON_SIZE).arg(dpr);
    QPixmap starPixmap;
    if (!QPixmapCache::find(starCacheKey, &starPixmap)) {
        QPixmap rawStar(":/icons/star.svg");
        if (!rawStar.isNull()) {
            starPixmap = rawStar.scaled(static_cast<int>(ICON_SIZE * dpr), static_cast<int>(ICON_SIZE * dpr),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
            starPixmap.setDevicePixelRatio(dpr);
            QPixmapCache::insert(starCacheKey, starPixmap);
        }
    }
    if (!starPixmap.isNull()) {
        starIcon->setPixmap(starPixmap);
    }
    starIcon->setFixedSize(ICON_SIZE, ICON_SIZE);

    QLabel *ratingLabel = new QLabel(QString::number(movie.rating, 'f', 1), statsRow);
    ratingLabel->setObjectName("movieCardRating");

    QLabel *commentsIcon = new QLabel(statsRow);
    const QString commentsCacheKey = QStringLiteral("icon:comments@%1x%2").arg(ICON_SIZE).arg(dpr);
    QPixmap commentsPixmap;
    if (!QPixmapCache::find(commentsCacheKey, &commentsPixmap)) {
        QPixmap rawComments(":/icons/comments.svg");
        if (!rawComments.isNull()) {
            commentsPixmap = rawComments.scaled(static_cast<int>(ICON_SIZE * dpr), static_cast<int>(ICON_SIZE * dpr),
                                                Qt::KeepAspectRatio, Qt::SmoothTransformation);
            commentsPixmap.setDevicePixelRatio(dpr);
            QPixmapCache::insert(commentsCacheKey, commentsPixmap);
        }
    }
    if (!commentsPixmap.isNull()) {
        commentsIcon->setPixmap(commentsPixmap);
    }
    commentsIcon->setFixedSize(ICON_SIZE, ICON_SIZE);

    QLabel *reviewCountLabel = new QLabel(QString::number(movie.reviewCount), statsRow);
    reviewCountLabel->setObjectName("movieCardReviews");

    statsLayout->addWidget(starIcon);
    statsLayout->addWidget(ratingLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(commentsIcon);
    statsLayout->addWidget(reviewCountLabel);

    infoLayout->addWidget(titleLabel);
    infoLayout->addWidget(genresLabel);
    infoLayout->addWidget(statsRow);

    cardLayout->addWidget(posterLabel);
    cardLayout->addWidget(infoWidget);
}

void MovieCard::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && rect().contains(event->pos())) {
        emit clicked(_movieId);
    }
    QFrame::mouseReleaseEvent(event);
}
