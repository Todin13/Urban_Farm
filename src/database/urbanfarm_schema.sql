--
-- PostgreSQL database dump
--

-- Dumped from database version 16.2 (Debian 16.2-1.pgdg120+2)
-- Dumped by pg_dump version 16.0

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: experiment; Type: TABLE; Schema: public; Owner: admin
--

CREATE TABLE public.experiment (
    id integer NOT NULL,
    name character varying(255)
);


ALTER TABLE public.experiment OWNER TO admin;

--
-- Name: experimentdata; Type: TABLE; Schema: public; Owner: admin
--

CREATE TABLE public.experimentdata (
    id integer NOT NULL,
    sensor_id integer,
    experiment_id integer
);


ALTER TABLE public.experimentdata OWNER TO admin;

--
-- Name: sensors; Type: TABLE; Schema: public; Owner: admin
--

CREATE TABLE public.sensors (
    id integer NOT NULL,
    version character varying(255)
);


ALTER TABLE public.sensors OWNER TO admin;

--
-- Name: sensorsdata; Type: TABLE; Schema: public; Owner: admin
--

CREATE TABLE public.sensorsdata (
    id integer NOT NULL,
    sensor_id integer,
    sensor_version character varying(255),
    plant_id integer,
    "time" timestamp without time zone,
    temperature double precision,
    humidity double precision,
    processed boolean DEFAULT false NOT NULL
);


ALTER TABLE public.sensorsdata OWNER TO admin;

--
-- Name: COLUMN sensorsdata.processed; Type: COMMENT; Schema: public; Owner: admin
--

COMMENT ON COLUMN public.sensorsdata.processed IS 'False if unprocessed
True if processed';


--
-- Name: warnings; Type: TABLE; Schema: public; Owner: admin
--

CREATE TABLE public.warnings (
    id integer NOT NULL,
    sensor_id integer,
    experiment_id integer,
    type character varying(255)
);


ALTER TABLE public.warnings OWNER TO admin;

--
-- Name: experiment experiment_pkey; Type: CONSTRAINT; Schema: public; Owner: admin
--

ALTER TABLE ONLY public.experiment
    ADD CONSTRAINT experiment_pkey PRIMARY KEY (id);


--
-- Name: experimentdata experimentdata_pkey; Type: CONSTRAINT; Schema: public; Owner: admin
--

ALTER TABLE ONLY public.experimentdata
    ADD CONSTRAINT experimentdata_pkey PRIMARY KEY (id);


--
-- Name: sensors sensors_pkey; Type: CONSTRAINT; Schema: public; Owner: admin
--

ALTER TABLE ONLY public.sensors
    ADD CONSTRAINT sensors_pkey PRIMARY KEY (id);


--
-- Name: sensorsdata sensorsdata_pkey; Type: CONSTRAINT; Schema: public; Owner: admin
--

ALTER TABLE ONLY public.sensorsdata
    ADD CONSTRAINT sensorsdata_pkey PRIMARY KEY (id);


--
-- Name: warnings warnings_pkey; Type: CONSTRAINT; Schema: public; Owner: admin
--

ALTER TABLE ONLY public.warnings
    ADD CONSTRAINT warnings_pkey PRIMARY KEY (id);


--
-- Name: experimentdata experimentdata_experiment_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: admin
--

ALTER TABLE ONLY public.experimentdata
    ADD CONSTRAINT experimentdata_experiment_id_fkey FOREIGN KEY (experiment_id) REFERENCES public.experiment(id);


--
-- Name: experimentdata experimentdata_sensor_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: admin
--

ALTER TABLE ONLY public.experimentdata
    ADD CONSTRAINT experimentdata_sensor_id_fkey FOREIGN KEY (sensor_id) REFERENCES public.sensors(id);


--
-- Name: sensorsdata sensorsdata_sensor_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: admin
--

ALTER TABLE ONLY public.sensorsdata
    ADD CONSTRAINT sensorsdata_sensor_id_fkey FOREIGN KEY (sensor_id) REFERENCES public.sensors(id);


--
-- Name: warnings warnings_experiment_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: admin
--

ALTER TABLE ONLY public.warnings
    ADD CONSTRAINT warnings_experiment_id_fkey FOREIGN KEY (experiment_id) REFERENCES public.experiment(id);


--
-- Name: warnings warnings_sensor_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: admin
--

ALTER TABLE ONLY public.warnings
    ADD CONSTRAINT warnings_sensor_id_fkey FOREIGN KEY (sensor_id) REFERENCES public.sensors(id);


--
-- PostgreSQL database dump complete
--

