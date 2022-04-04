import React from 'react';
import './App.css';
import Row from './Row';
import requests from './request';
import Nav from './Nav';
import Banner from './Banner'

function App() {
  debugger;
  return (
    <div className="app">
      <Nav/>
      <Banner/>

      <Row title="NETFLIX ORIGINALS" fetchUrl={requests.fetchNetflixOriginals} isLargeRow/>
      <Row title="Trending Now" fetchUrl={requests.fetchTrending}/>
      <Row title="Action" fetchUrl={requests.fetchActionMovies}/>
      <Row title="Documentaries" fetchUrl={requests.fetchDocumentaries}/>
      <Row title="Romance" fetchUrl={requests.fetchRomanceMovies}/>
      <Row title="Horror" fetchUrl={requests.fetchHorrorMovies}/>
      <Row title="Comedy" fetchUrl={requests.fetchComedyMovies}/>
    </div> 
  );
}

export default App;
