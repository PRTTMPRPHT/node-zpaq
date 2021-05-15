module.exports = {
    modulePathIgnorePatterns: [
        "<rootDir>/dist/"
    ],
    transform: {
        "^.+\\.tsx?$": "ts-jest"
    },
    globals: {
        "ts-jest": {
            tsconfig: "jest.tsconfig.json"
        }
    },
    collectCoverageFrom: [
        "src/**/*.ts",
        "!src/**/*.spec.ts",
        "!src/**/index.ts"
    ]
};
