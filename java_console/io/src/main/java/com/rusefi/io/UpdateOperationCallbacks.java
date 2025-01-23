package com.rusefi.io;

public interface UpdateOperationCallbacks {
    void log(String message, boolean breakLineOnTextArea, boolean sendToLogger);

    default void logLine(final String message) {
        log(message, true, true);
    }

    void done();
    void error();

    void clear();

    UpdateOperationCallbacks DUMMY = new UpdateOperationCallbacks() {
        @Override
        public void log(final String message, final boolean breakLineOnTextArea, boolean sendToLogger) {
        }

        @Override
        public void done() {
        }

        @Override
        public void error() {
        }

        @Override
        public void clear() {
        }
    };
}
