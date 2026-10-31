# Remote Environmental Survey and Mapping of an Unseen Room

This repository stores research, references, design notes, and report material for a CEGEP engineering project. The project concerns a remotely operated mobile unit that surveys one unfamiliar enclosed room without requiring the operator to enter it.

## Current project scope

The proposed system includes:

- remote operation from a phone with a live video feed;
- a 2D map of one room with measurement locations;
- temperature, humidity, and relative gas measurements;
- a survey summary with hazard indications on a watch; and
- a low-fidelity prototype using breadboard or perfboard construction.

The project does not claim certified gas detection or room-safety certification. Autonomous navigation, 3D mapping, multiple-room mapping, cloud storage, long-term monitoring, and PCB manufacture are outside the current scope.

## Requirements

The project requirement identifiers are preserved in `research/requirements.md`:

- REQ-RS-01 through REQ-RS-10

## Repository structure

- `paper/` - project source documents and the current Stage 1 Concept and Feasibility Report in `paper/stage-1-concept-and-feasibility-report/`
- `photos/` - research photos and Version 1 watch Wi-Fi/display evidence
- `research/` - research notes, sources, requirement tracking, and future evidence
- `src/` - project source code, separated into watch code and a placeholder for future rover code

## Evidence policy

Measurements, test results, logbook entries, and technical evidence should be added only after they have been collected or verified. Planned tests must be clearly labelled as plans, not results.

