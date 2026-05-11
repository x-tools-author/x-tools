---
description: "Review Qt6 C++ code for correctness, best practices, and performance"
---

# Qt C++ Code Review

Review Qt6 C++ code for the following rules. Report only confirmed
findings with file path and line number.

## API and naming (API, HDR, PRP)

- Naming consistency with Qt conventions: `timeout` not `timeOut`,
  `size()` not `count()` (API-3)
- `get`-prefix means user interaction or decomposition, not getters (API-5)
- Q_PROPERTY FINAL on new classes unless override intended (PRP-1)
- No properties with same name as meta-methods (PRP-3)
- Protect min/max: `(std::min)(a,b)` (HDR-3)

## Model contracts (MDL)

- Structural changes must use begin/end signals, not layoutChanged (MDL-1)
- dataChanged must pass specific roles, not empty vector (MDL-2)
- setData() must emit dataChanged before returning true (MDL-3)
- beginRemoveRows with first > last violates contract (MDL-4)
- begin/end signal pairs must be balanced in each code path (MDL-6)
- roleNames() must match data() switch cases (MDL-8)

## Resource lifecycle (LCY)

- deleteLater() on QNetworkReply in every finished handler (LCY-1)
- QObject-derived objects with `new` need parent or explicit
  lifecycle management (LCY-2)
- No side-effectful expressions inside Q_ASSERT (LCY-3)
- No Q_ASSERT(ptr) as sole null guard before dereference (LCY-4)
- Cap unbounded container growth (LCY-5)

## Thread safety (THR)

- No writing QObject members from QtConcurrent::run without
  synchronization (THR-1)
- No DirectConnection signals from worker to main thread (THR-2)
- No mutating QAbstractItemModel from background threads (THR-3)
- Consistent mutex protection across all code paths (THR-4)

## Error handling (ERR)

- Check QFile::open() return before read/write (ERR-1)
- Check QJsonDocument::fromJson() with isNull()/isObject() (ERR-2)
- Check QNetworkReply::error() before readAll() (ERR-3)
- Use https:// not http:// (ERR-4)
- Set QNetworkRequest::setTransferTimeout() (ERR-5)
- Match QString::arg() placeholder count to .arg() calls (ERR-6)
- Handle QNetworkAccessManager::sslErrors signal (ERR-9)
- Validate input lengths from untrusted sources (ERR-11)

## Exceptions and noexcept (NXC)

- noexcept functions: flag only if a clear throwing path is found
  (allocation, precondition Q_ASSERT, calls to throwing functions).
  Do not flag speculatively (NXC-1, Lakos Rule)
- Q_ASSERT checking preconditions is incompatible with noexcept;
  Q_ASSERT checking invariants is acceptable (NXC-3)

## Performance (PRF)

- No QRegularExpression constructed inside loops (PRF-1)
- Use `const auto&` in range-for over shared containers to avoid
  COW detach (PRF-3)
- Use .value() not operator[] for reads on shared QHash/QMap (PRF-4)
- Cheap early-exit checks before expensive operations (PRF-5)
- Do not use QMap for small fixed-size constant data (PRF-8)
- Add re-entrancy guards on methods that emit signals which could
  trigger recursive calls (PRF-10)

## Polymorphic classes (PLY)

- Q_DISABLE_COPY_MOVE on polymorphic classes (PLY-2)
- Virtual functions marked by exactly ONE of virtual, override,
  final (PLY-4)

## Timeouts (TMO)

- No ints for timeouts: use QDeadlineTimer or std::chrono (TMO-1)
