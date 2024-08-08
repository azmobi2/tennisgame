#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

const int windowWidth = 320;
const int windowHeight = 240;
const float playerWidth = 10;
const float playerHeight = 40;
const float ballRadius = 5;
const float ballInitialSpeed = 3.0f;
const float playerSpeed = 4.0f;
const float aiSpeed = 2.0f;  // Velocidade inicial do jogador automático
const float aiSpeedIncreased = 3.5f;  // Velocidade aumentada para o jogador automático
const float aiSpeedDecreased = 1.0f;  // Velocidade diminuída para o jogador automático
const int maxScore = 30;

void resetGame(sf::RectangleShape& player1, sf::RectangleShape& player2, sf::CircleShape& ball, sf::Vector2f& ballVelocity, int& player1ScoreValue, int& player2ScoreValue, bool& gameOver) {
    player1.setPosition(10, windowHeight / 2 - playerHeight / 2);
    player2.setPosition(windowWidth - playerWidth - 10, windowHeight / 2 - playerHeight / 2);
    ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
    ballVelocity = sf::Vector2f(ballInitialSpeed, ballInitialSpeed);
    player1ScoreValue = 0;
    player2ScoreValue = 0;
    gameOver = false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Jogo de Tênis");
    window.setFramerateLimit(60);

    sf::RectangleShape player1(sf::Vector2f(playerWidth, playerHeight));
    player1.setFillColor(sf::Color::White);
    player1.setPosition(10, windowHeight / 2 - playerHeight / 2);

    sf::RectangleShape player2(sf::Vector2f(playerWidth, playerHeight));
    player2.setFillColor(sf::Color::White);
    player2.setPosition(windowWidth - playerWidth - 10, windowHeight / 2 - playerHeight / 2);

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
                    player1.setPosition(player1.getPosition().x, touchY - playerHeight / 2);
                }
            } else if (event.type == sf::Event::TouchBegan && gameOver) {
                if (playAgainButton.getGlobalBounds().contains(event.touch.x, event.touch.y)) {
                    resetGame(player1, player2, ball, ballVelocity, player1ScoreValue, player2ScoreValue, gameOver);
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

            if (ball.getPosition().y < player2.getPosition().y + playerHeight / 2 - 10) {
                player2.move(0, -currentAiSpeed);
            } else if (ball.getPosition().y > player2.getPosition().y + playerHeight / 2 + 10) {
                player2.move(0, currentAiSpeed);
            }

            if (player1.getPosition().y < 0) player1.setPosition(player1.getPosition().x, 0);
            if (player1.getPosition().y > windowHeight - playerHeight) player1.setPosition(player1.getPosition().x, windowHeight - playerHeight);

            if (player2.getPosition().y < 0) player2.setPosition(player2.getPosition().x, 0);
            if (player2.getPosition().y > windowHeight - playerHeight) player2.setPosition(player2.getPosition().x, windowHeight - playerHeight);

            ball.move(ballVelocity);

            if (ball.getPosition().y <= 0 || ball.getPosition().y >= windowHeight - ballRadius * 2) {
                ballVelocity.y = -ballVelocity.y;
                hitSound.play();
            }

            if (ball.getGlobalBounds().intersects(player1.getGlobalBounds()) || ball.getGlobalBounds().intersects(player2.getGlobalBounds())) {
                ballVelocity.x = -ballVelocity.x;
                hitSound.play();
            }

            if (ball.getPosition().x <= 0) {
                player2ScoreValue++;
                ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
                ballVelocity = sf::Vector2f(ballInitialSpeed, ballInitialSpeed);
            } else if (ball.getPosition().x >= windowWidth - ballRadius * 2) {
                player1ScoreValue++;
                ball.setPosition(windowWidth / 2 - ballRadius, windowHeight / 2 - ballRadius);
                ballVelocity = sf::Vector2f(-ballInitialSpeed, ballInitialSpeed);
            }

            if (player1ScoreValue >= maxScore || player2ScoreValue >= maxScore) {
                gameOver = true;
                std::string winner = (player1ScoreValue >= maxScore) ? "Jogador 1 Venceu!" : "Jogador Automático Venceu!";
                gameOverText.setString(winner);
            }

            player1ScoreText.setString(std::to_string(player1ScoreValue));
            player2ScoreText.setString(std::to_string(player2ScoreValue));
        }

        window.clear(sf::Color::Black);
        window.draw(player1);
        window.draw(player2);
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
    return 0; // programa criado por ChatGPT IA, adaptado para Cxxdroid por Alexandre Zanca Bacich.
}