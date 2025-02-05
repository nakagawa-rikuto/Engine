#include "CardManager.h"

void CardManager::Initialize(std::vector<std::vector<int>> cardData, CameraManager* cameraManager) {

	rows = cardData.size();
	cols = cardData[0].size();

	cards_.resize(rows);
	for (auto& row : cards_) {
		row.resize(cols);
	}

	const float spacing = 5.0f;                   // モデル間の間隔

	const Vector3 basePosition(static_cast<float>((cols - 1)) / 2 * -spacing, static_cast<float>((rows - 1)) / 2 * spacing, 0.0f); // 基準となる位置

	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {

			Vector3 position(basePosition.x + x * spacing, basePosition.y + y * -spacing, basePosition.z);

			std::string type = std::to_string(cardData[y][x]);

#pragma region
			
			const std::string& CardModel = "Card" + type;
			
#pragma endregion

			std::unique_ptr<Card> newCard = std::make_unique<Card>();

			newCard->Initialize(CardModel, cardData[y][x], position, cameraManager->GetActiveCamera());

			cards_[y][x] = std::move(newCard);
		}
	}

	cameraManager_ = cameraManager;
}

void CardManager::Update(Vector2 mousePosition) {
	isFlipCard = false;

	CheckCursorCardCollision(mousePosition);

	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			if (cards_[y][x]->GetCurrentState() != Card::CardState::obtained) {
				cards_[y][x]->Update(cameraManager_->GetActiveCamera());

				cards_[y][x]->SetScale({1.0f, 1.0f, 1.0f});
			}
		}
	}

	allObtainedCardCount = CountState(Card::CardState::obtained);
	CheckFrontPair();
}

void CardManager::Darw() {

	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			cards_[y][x]->Draw();
		}
	}
}

void CardManager::CheckFrontPair() {

	// 記録用変数
	int cardType[2];
	int cardX[2];
	int cardZ[2];

	int pairIndex = 0;

	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			// 表を向いた状態だったら変数に記録
			if (cards_[y][x]->GetCurrentState() == Card::CardState::front) {
				cardType[pairIndex] = cards_[y][x]->GetCardType();
				cardX[pairIndex] = x;
				cardZ[pairIndex] = y;

				if (pairIndex == 2) {
					break;
				}

				++pairIndex;
			}
		}
	}

	if (pairIndex == 2) {

		if (cardType[0] == cardType[1]) {

			for (int i = 0; i < 2; ++i) {

				cards_[cardZ[i]][cardX[i]]->SetCurrentState(Card::CardState::show);

				cards_[cardZ[i]][cardX[i]]->RequestState(Card::CardState::obtained);
			}

			EightDirectionCheck(cardZ, cardX);
		} else if (cardType[0] != cardType[1]) {

			for (int i = 0; i < 2; ++i) {

				cards_[cardZ[i]][cardX[i]]->SetCurrentState(Card::CardState::show);

				cards_[cardZ[i]][cardX[i]]->RequestState(Card::CardState::back);
			}
		}


		++stepCount;

		if (stepCount == 10)
		{
			step10obtainedCount = CountState(Card::CardState::obtained);
		}

		if (stepCount == 15)
		{
			step15obtainedCount = CountState(Card::CardState::obtained);
		}
	}
}

bool CardManager::AllCardsObtained() {
	for (const auto& row : cards_) {
		for (const auto& card : row) {
			if (card->GetCurrentState() != Card::CardState::obtained) {
				return false;
			}
		}
	}
	return true;
}

void CardManager::CardDataRefresh(std::vector<std::vector<int>> cardData)
{
	stepCount = 0;

	// 同時に挟んで消した数
	eraseCardMaxCount = 0;

	// 挟んで消したカード
	eraseCardCount = 0;

	eraseCount = 0;

	// 
	allObtainedCardCount = 0;
	
	step10obtainedCount = 0;

	step15obtainedCount = 0;

	cards_[0].clear();
	cards_.clear();

	rows = cardData.size();
	cols = cardData[0].size();

	cards_.resize(rows);
	for (auto& row : cards_) {
		row.resize(cols);
	}

	const float spacing = 5.0f;                   // モデル間の間隔

	const Vector3 basePosition(static_cast<float>((cols - 1)) / 2 * -spacing, static_cast<float>((rows - 1)) / 2 * spacing, 0.0f); // 基準となる位置

	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {

			Vector3 position(basePosition.x + x * spacing, basePosition.y + y * -spacing, basePosition.z);

			std::string type = std::to_string(cardData[y][x]);

#pragma region

			const std::string& CardModel = "Card" + type;

#pragma endregion

			std::unique_ptr<Card> newCard = std::make_unique<Card>();

			newCard->Initialize(CardModel, cardData[y][x], position, cameraManager_->GetActiveCamera());

			cards_[y][x] = std::move(newCard);
		}
	}
}

void CardManager::CheckCursorCardCollision(Vector2 mousePosition) {
	if (!CountStateCard(Card::CardState::front) || !CountStateCard(Card::CardState::show)) {
		return;
	}

	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			Vector2 diffVector = cards_[y][x]->GetScreenPosition() - mousePosition;

			float len = sqrtf(diffVector.x * diffVector.x + diffVector.y * diffVector.y);

			if (len < 50.0f && cards_[y][x]->GetCurrentState() == Card::CardState::back) {
				cards_[y][x]->SetScale({1.3f, 1.3f, 1.3f});

				if (Mii::PushMouse(MouseButtonType::Left)) {
					if (triggerLeft_) {

						cards_[y][x]->RequestState(Card::CardState::front);

						// フラグを立てる
						isFlipCard = true;

						triggerLeft_ = false;
					} else {
						triggerLeft_ = true;
					}
				}
			} 
		}
	}
}

bool CardManager::CountStateCard(Card::CardState state) {
	int count = 0;
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			if (cards_[y][x]->GetCurrentState() == state || cards_[y][x]->GetRequestState() == state) {
				++count;
			}
		}
	}

	if (count < 2) {
		return true;
	}

	return false;
}

int CardManager::EightDirectionCheck(int yIndex[2], int xIndex[2]) {
	// 差
	int diffY = yIndex[1] - yIndex[0];
	int diffX = xIndex[1] - xIndex[0];

	int obtainedCount = 0;

	// 行が同じ場合
	if (diffY == 0) {

		if (std::abs(diffX) > 1) {
			int addNum = diffX / std::abs(diffX);

			int addXIndex = xIndex[0];

			for (int i = 0; i < std::abs(diffX) - 1; ++i) {

				addXIndex += addNum;

				if (cards_[yIndex[0]][addXIndex]->GetCurrentState() == Card::CardState::back) {
					cards_[yIndex[0]][addXIndex]->SetCurrentState(Card::CardState::show);

					cards_[yIndex[0]][addXIndex]->RequestState(Card::CardState::obtained);

					++obtainedCount;
				}
			}
		}
	}
	// 列が同じ場合
	else if (diffX == 0) {

		if (std::abs(diffY) > 1) {
			int addNum = diffY / std::abs(diffY);

			int addYIndex = yIndex[0];

			for (int i = 0; i < std::abs(diffY) - 1; ++i) {

				addYIndex += addNum;

				if (cards_[addYIndex][xIndex[0]]->GetCurrentState() == Card::CardState::back) {
					cards_[addYIndex][xIndex[0]]->SetCurrentState(Card::CardState::show);

					cards_[addYIndex][xIndex[0]]->RequestState(Card::CardState::obtained);
					++obtainedCount;
				}
			}
		}
	}
	// 斜めだった場合
	else if (std::abs(diffY) == std::abs(diffX)) {

		int yAdd = diffY / std::abs(diffY);
		int xAdd = diffX / std::abs(diffX);

		for (int i = 0; i < std::abs(diffY) - 1; ++i) {
			yIndex[0] += yAdd;
			xIndex[0] += xAdd;

			if (cards_[yIndex[0]][xIndex[0]]->GetCurrentState() == Card::CardState::back) {
				cards_[yIndex[0]][xIndex[0]]->SetCurrentState(Card::CardState::show);

				cards_[yIndex[0]][xIndex[0]]->RequestState(Card::CardState::obtained);
				++obtainedCount;
			}
		}
	}

	if (eraseCardMaxCount < obtainedCount)
	{
		eraseCardMaxCount = obtainedCount;
	}

	if (obtainedCount > 0)
	{
		++eraseCount;
	}

	eraseCardCount += obtainedCount;

	return obtainedCount;
}

int CardManager::CountState(Card::CardState state)
{
	int count = 0;
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			if (cards_[y][x]->GetCurrentState() == state || cards_[y][x]->GetRequestState() == state) {
				++count;
			}
		}
	}

	return count;
}

///-------------------------------------------/// 
/// 詰み検索
///-------------------------------------------///
bool CardManager::Checkmate() {
	/// ===カウントの初期化=== ///
	int numOneCount = 0;
	int numTwoCount = 0;
	int numThreeCount = 0;
	int numFourCount = 0;
	int numFiveCount = 0;

	/// ===検索=== ///
	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {
			if (cards_[y][x]->GetCurrentState() == Card::CardState::back ||
				cards_[y][x]->GetCurrentState() == Card::CardState::front ||
				cards_[y][x]->GetCurrentState() == Card::CardState::show) {
				// 状態チェック
				if (cards_[y][x]->GetCardType() == 1) {
					numOneCount++;
				} else if (cards_[y][x]->GetCardType() == 2) {
					numTwoCount++;
				} else if (cards_[y][x]->GetCardType() == 3) {
					numThreeCount++;
				} else if (cards_[y][x]->GetCardType() == 4) {
					numFourCount++;
				} else if (cards_[y][x]->GetCardType() == 5) {
					numFiveCount++;
				}
			}
		}
	}
	// 詰みチェック
	if (numOneCount < 2 && numTwoCount < 2 && numThreeCount < 2 && numFourCount < 2 && numFiveCount < 2) {
		return true;
	}
	// 詰み出なければfalse
	return false;
}

