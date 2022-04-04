import axios from 'axios';

const instance = axios.create({
    baseURL: "https://api.themoviedb.org/3",
    trailerUrl: "https://youtube.com/watch?v="
})

export default instance;