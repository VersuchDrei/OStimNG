#include "GameUtil.h"

namespace GameAPI {
    namespace GameUtil {
        void forEachReferenceInRange(const RE::NiPoint3& center, float radius, std::function<RE::BSContainer::ForEachResult(RE::TESObjectREFR&)> callback) {
            auto TES = RE::TES::GetSingleton();
            if (!TES) return;

            auto cell = TES->interiorCell;
            if (cell) {
                cell->ForEachReferenceInRange(center, radius, [&](RE::TESObjectREFR& ref) { return callback(ref); });
                return;
            }

            auto gridCellArray = TES->gridCells;
            if (!gridCellArray) return;

            auto gridLength = gridCellArray->length;
            if (gridLength > 0) {
                std::uint32_t x = 0;
                std::uint32_t y = 0;
                float yPlus = center.y + radius;
                float yMinus = center.y - radius;
                float xPlus = center.x + radius;
                float xMinus = center.x - radius;

                for (x = 0, y = 0; (x < gridLength && y < gridLength); x++, y++) {
                    cell = gridCellArray->GetCell(x, y);
                    if (cell && cell->IsAttached()) {
                        auto cellCoords = cell->GetCoordinates();
                        if (!cellCoords) continue;

                        float worldX = cellCoords->worldX;
                        float worldY = cellCoords->worldY;

                        if (worldX < xPlus && (worldX + 4096.0) > xMinus && worldY < yPlus &&
                            (worldY + 4096.0) > yMinus) {
                            cell->ForEachReferenceInRange(center, radius, [&](RE::TESObjectREFR& a_cellRef) { return callback(a_cellRef); });
                        }
                    }
                }
            }
        }
    }
}