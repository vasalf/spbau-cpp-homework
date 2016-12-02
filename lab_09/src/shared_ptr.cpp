#include "shared_ptr.h"

shared_ptr::shared_ptr(Matrix *obj) {
    storage_  = new Storage(obj);
};

shared_ptr& shared_ptr::operator=(shared_ptr other) {
    if (ptr() == other.ptr())
        return *this;
    if (storage_->getCounter() != 0) {
        storage_->decr();
        if (storage_->getCounter() == 0)
            delete storage_;
    }
    storage_ = other.storage_;
    storage_->incr();
    return *this;
}

shared_ptr::shared_ptr(const shared_ptr& other) {
    storage_ = other.storage_;
    storage_->incr();
}

shared_ptr::~shared_ptr() {
    storage_->decr();
    if (storage_->getCounter() == 0)
        delete storage_;
}

Matrix* shared_ptr::ptr() const {
    return storage_->getObject();
}

bool shared_ptr::isNull() const {
    return ptr() == NULL;
}

void shared_ptr::reset(Matrix* obj) {
    *this = shared_ptr(obj);
}

Matrix* shared_ptr::operator->() const {
    return ptr();
}

Matrix& shared_ptr::operator*() const {
    return *ptr();
}

shared_ptr::Storage::Storage(Matrix *mtx) {
    data_ = mtx;
    ref_count_ = 1;
}

shared_ptr::Storage::~Storage() {
    delete data_;
}

void shared_ptr::Storage::incr() {
    ref_count_++;
}

void shared_ptr::Storage::decr() {
    ref_count_--;
}

int shared_ptr::Storage::getCounter() const {
    return ref_count_;
}

Matrix* shared_ptr::Storage::getObject() const {
    return data_;
}

