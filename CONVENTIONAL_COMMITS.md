# Conventional Commits

This document defines our team's commit message convention based on the Conventional Commits specification, with a few
team-specific examples and the required issue-reference style `(#2)`.

**Commit message format**

- Format: `type(optional-scope): short description`

**Components**

- `type`: one of the allowed types (see reference below).
- `optional-scope`: short area or module (no spaces), e.g. `auth`, `ui`, `api`.
- `short description` (subject): imperative, max ~72 chars, no trailing period.

**Allowed types and descriptions**

- `feat`: a new feature for the user.
- `fix`: a bug fix.
- `docs`: documentation only changes.
- `style`: formatting, missing semicolons, white-space, etc. (no code change).
- `refactor`: code change that neither fixes bug nor adds feature.
- `perf`: a code change that improves performance.
- `build`: changes that affect the build system or external dependencies (CMakeLists, etc.).
- `chore`: other changes that don’t modify src or test files (e.g., tooling updates).
- `revert`: reverts a previous commit (use with the reverted commit hash in body).

**Issue referencing**

- When referencing issues in commits, always use the parenthetical short form: `(#2)`.
- Examples:
    - Subject-level: `fix(api): handle null responses (#99)`

**Examples**

- `feat(ui): add dark mode toggle (#17)` - adds new feature, references issue 17.
- `fix(parser): avoid crash on empty input (#2)` - bugfix referencing issue 2.
- `docs: add API usage examples (#2)` - documentation change referencing issue 2.
- `perf: optimize image decoding` - performance improvement.
- `refactor(auth): extract token handler` - refactor with scope.
