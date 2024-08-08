#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

const int windowWidth = 320;
const int windowHeight = 240;
const float playerWidth = windowWidth * 0.2f; // Ajustado para 0.2 da largura da tela
const float playerHeight = windowHeight * 0.2f; // Ajustado para 0.2 da altura da tela
const float ballRadius = 2.25;
const float ballInitialSpeed = 3.0f;
const float playerSpeed = 4.0f;
const float aiSpeed = 2.0f;
const float aiSpeedIncreased = 3.5f;
const float aiSpeedDecreased = 1.0f;
const int maxScore = 30;

void resetGame(sf::Sprite& player1Sprite, sf::Sprite& player2Sprite, sf::CircleShape& ball, sf::Vector2f& ballVelocity, int& player1ScoreValue, int& player2ScoreValue, bool& gameOver) {
    player1Sprite.setPosition(10, windowHeight / 2 - playerHeight / 2);
    player2Sprite.setPosition(windowWidth - playerWidth - 10, windowHeight / 2 - playerHeight / 2);
    ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
    ballVelocity = sf::Vector2f(ballInitialSpeed, ballInitialSpeed);
    player1ScoreValue = 0;
    player2ScoreValue = 0;
    gameOver = false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Jogo de Tênis");
    window.setFramerateLimit(60);

    // Carregar texturas
    sf::Texture backgroundTexture, player1Texture, player2Texture;
    if (!backgroundTexture.loadFromFile("fundo_quadra.png")) {
        std::cerr << "Erro ao carregar a textura do fundo!" << std::endl;
        return -1;
    }
    if (!player1Texture.loadFromFile("jogador1fundo.png")) {
        std::cerr << "Erro ao carregar a textura do jogador 1!" << std::endl;
        return -1;
    }
    if (!player2Texture.loadFromFile("jogador2fundo.png")) {
        std::cerr << "Erro ao carregar a textura do jogador 2!" << std::endl;
        return -1;
    }

    // Criar sprites
    sf::Sprite background(backgroundTexture);
    sf::Sprite player1Sprite(player1Texture);
    sf::Sprite player2Sprite(player2Texture);

    // Ajustar a escala da textura do fundo
    background.setScale(
        static_cast<float>(windowWidth) / backgroundTexture.getSize().x,
        static_cast<float>(windowHeight) / backgroundTexture.getSize().y
    );

    // Ajustar a escala da textura do jogador 1
    player1Sprite.setScale(playerWidth / player1Texture.getSize().x, playerHeight / player1Texture.getSize().y);
    player1Sprite.setPosition(10, windowHeight / 2 - playerHeight / 2);

    // Ajustar a escala da textura do jogador 2
    player2Sprite.setScale(playerWidth / player2Texture.getSize().x, playerHeight / player2Texture.getSize().y);
    player2Sprite.setPosition(windowWidth - playerWidth - 10, windowHeight / 2 - playerHeight / 2);

    sf::CircleShape ball(ballRadius);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Erro ao carregar fonte!" << std::endl;
        return -1;
    }

    sf::Text player1ScoreText, player2ScoreText, instruction, gameOverText, playAgainText;
    player1ScoreText.setFont(font);
    player1ScoreText.setCharacterSize(14);
    player1ScoreText.setFillColor(sf::Color::White);
    player1ScoreText.setPosition(windowWidth / 2 - 40, 10);

    player2ScoreText.setFont(font);
    player2ScoreText.setCharacterSize(14);
    player2ScoreText.setFillColor(sf::Color::White);
    player2ScoreText.setPosition(windowWidth / 2 + 20, 10);

    instruction.setFont(font);
    instruction.setCharacterSize(10);
    instruction.setFillColor(sf::Color::White);
    instruction.setString("Movimentar o jogador 1 com o toque no lado esquerdo superior, acima do jogador 1.");
    instruction.setPosition(10, windowHeight - 15);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(14);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(windowWidth / 2 - 80, windowHeight / 2 - 20);

    sf::RectangleShape playAgainButton(sf::Vector2f(80, 20));
    playAgainButton.setFillColor(sf::Color::White);
    playAgainButton.setPosition(windowWidth / 2 - 40, windowHeight - 40);

    playAgainText.setFont(font);
    playAgainText.setCharacterSize(10);
    playAgainText.setFillColor(sf::Color::Black);
    playAgainText.setString("Play Again");
    playAgainText.setPosition(windowWidth / 2 - 30, windowHeight - 37);

    int player1ScoreValue = 0, player2ScoreValue = 0;
    sf::Vector2f ballVelocity(ballInitialSpeed, ballInitialSpeed);

    sf::SoundBuffer hitSoundBuffer;
    if (!hitSoundBuffer.loadFromFile("hit_sound.wav")) {
        std::cerr << "Erro ao carregar som!" << std::endl;
        return -1;
    }

    sf::Sound hitSound;
    hitSound.setBuffer(hitSoundBuffer);

    bool gameOver = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if ((event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchMoved) && !gameOver) {
                if (event.touch.x < windowWidth / 2) {
                    int touchY = event.touch.y;
                    player1Sprite.setPosition(10, touchY - playerHeight / 2);
                }
            } else if (event.type == sf::Event::TouchBegan && gameOver) {
                if (playAgainButton.getGlobalBounds().contains(event.touch.x, event.touch.y)) {
                    resetGame(player1Sprite, player2Sprite, ball, ballVelocity, player1ScoreValue, player2ScoreValue, gameOver);
                }
            }
        }

        if (!gameOver) {
            float currentAiSpeed = aiSpeed;
            if (player1ScoreValue >= 26 && player2ScoreValue < 26) {
                currentAiSpeed = aiSpeedIncreased;
            } else if (player2ScoreValue >= 26 && player1ScoreValue < 26) {
                currentAiSpeed = aiSpeedDecreased;
            }

            if (ball.getPosition().y < player2Sprite.getPosition().y + playerHeight / 2 - 10) {
                player2Sprite.move(0, -currentAiSpeed);
            } else if (ball.getPosition().y > player2Sprite.getPosition().y + playerHeight / 2 + 10) {
                player2Sprite.move(0, currentAiSpeed);
            }

            if (player1Sprite.getPosition().y < 0) player1Sprite.setPosition(player1Sprite.getPosition().x, 0);
            if (player1Sprite.getPosition().y > windowHeight - playerHeight) player1Sprite.setPosition(player1Sprite.getPosition().x, windowHeight - playerHeight);

            if (player2Sprite.getPosition().y < 0) player2Sprite.setPosition(player2Sprite.getPosition().x, 0);
            if (player2Sprite.getPosition().y > windowHeight - playerHeight) player2Sprite.setPosition(player2Sprite.getPosition().x, windowHeight - playerHeight);

            ball.move(ballVelocity);

            if (ball.getPosition().y <= 0 || ball.getPosition().y >= windowHeight - ballRadius * 2) {
                ballVelocity.y = -ballVelocity.y;
                hitSound.play();
            }

            // Colisão com os jogadores
            if (ball.getGlobalBounds().intersects(player1Sprite.getGlobalBounds())) {
                ballVelocity.x = -ballVelocity.x;
                hitSound.play();
            } else if (ball.getGlobalBounds().intersects(player2Sprite.getGlobalBounds())) {
                ballVelocity.x = -ballVelocity.x;
                hitSound.play();
            }

            // Verificar se a bola saiu da tela pelos lados esquerdo ou direito
            if (ball.getPosition().x <= 0) {
                player2ScoreValue++;
                if (player2ScoreValue >= maxScore) {
                    gameOverText.setString("Player 2 Wins!");
                    gameOver = true;
                } else {
                    ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
                    ballVelocity = sf::Vector2f(-ballInitialSpeed, ballInitialSpeed);
                }
            } else if (ball.getPosition().x >= windowWidth - ballRadius * 2) {
                player1ScoreValue++;
                if (player1ScoreValue >= maxScore) {
                    gameOverText.setString("Player 1 Wins!");
                    gameOver = true;
                } else {
                    ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
                    ballVelocity = sf::Vector2f(ballInitialSpeed, ballInitialSpeed);
                }
            }

            player1ScoreText.setString(std::to_string(player1ScoreValue));
            player2ScoreText.setString(std::to_string(player2ScoreValue));
        }

        window.clear();
        window.draw(background);
        window.draw(player1Sprite);
        window.draw(player2Sprite);
        window.draw(ball);
        window.draw(player1ScoreText);
        window.draw(player2ScoreText);
        window.draw(instruction);

        if (gameOver) {
            window.draw(gameOverText);
            window.draw(playAgainButton);
            window.draw(playAgainText);
        }

        window.display();
    }

    return 0;
}